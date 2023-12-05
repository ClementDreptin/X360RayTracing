#include "pch.h"
#include "Renderer/Image.h"

#include <AtgUtil.h>

#include "Renderer/D3DDevice.h"

extern float g_DisplayWidth;
extern float g_DisplayHeight;

bool Image::s_ShadersInitialized = false;
D3DVertexShader *Image::s_pVertexShader = nullptr;
D3DPixelShader *Image::s_pPixelShader = nullptr;

Image::Image()
    : m_Initialized(false), m_pTexture(nullptr)
{
}

Image::~Image()
{
    if (m_pTexture != nullptr)
        m_pTexture->Release();
}

void Image::Render(const Props &props)
{
    // Check if the world view projection matrix and/or the vertex buffer need to be updated
    bool needToUpdateWorldViewProjectionMatrix = m_Props.X != props.X || m_Props.Y != props.Y;
    bool needToUpdateVertexBuffer = m_Props.Width != props.Width || m_Props.Height != props.Height;

    m_Props = props;

    // If this is the first time Render is called, just initialize the image and return
    if (!m_Initialized)
    {
        Init();
        return;
    }

    // Perform the updates if needed
    if (needToUpdateWorldViewProjectionMatrix)
        CalculateWorldViewProjectionMatrix();

    if (needToUpdateVertexBuffer)
        UpdateVertexBuffer();

    PopulateTexture();

    // Initialize default device states
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    // Render the image
    g_pd3dDevice->SetTexture(0, m_pTexture);
    g_pd3dDevice->SetVertexDeclaration(m_VertexBuffer.GetVertexDeclaration());
    g_pd3dDevice->SetStreamSource(0, &m_VertexBuffer, 0, sizeof(ImageVertex));
    g_pd3dDevice->SetVertexShader(s_pVertexShader);
    g_pd3dDevice->SetVertexShaderConstantF(0, reinterpret_cast<float *>(&m_WVPMatrix), 4);
    g_pd3dDevice->SetPixelShader(s_pPixelShader);
    g_pd3dDevice->DrawPrimitive(D3DPT_QUADLIST, 0, 1);
}

HRESULT Image::Init()
{
    HRESULT hr = S_OK;

    // Set up the matrices for orthographic projection
    m_ViewMatrix = XMMatrixIdentity();
    m_ProjectionMatrix = XMMatrixOrthographicOffCenterLH(0.0f, g_DisplayWidth, 0.0f, g_DisplayHeight, -1.0f, 1.0f);
    CalculateWorldViewProjectionMatrix();

    if (!s_ShadersInitialized)
    {
        hr = InitShaders();
        if (FAILED(hr))
            return hr;
    }

    // Create the texture
    hr = g_pd3dDevice->CreateTexture(
        m_Props.Width,
        m_Props.Height,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_pTexture,
        nullptr
    );
    if (FAILED(hr))
    {
        Log::Error("Couldn't create texture");
        return hr;
    }

    // Create the vertices and the vertex buffer
    // Since the Y axis goes upwards, if we want a height increase to make our
    // rectangle grow downwards along the Y axis, we need to substract its height
    // to the Y coordinate of each vertex.
    float width = static_cast<float>(m_Props.Width);
    float height = static_cast<float>(m_Props.Height);
    ImageVertex vertices[] = {
        ImageVertex(0.0f, 0.0f - height, 0.0f, 0.0f, 1.0f),    // Bottom Left
        ImageVertex(0.0f, height - height, 0.0f, 0.0f, 0.0f),  // Top Left
        ImageVertex(width, height - height, 0.0f, 1.0f, 0.0f), // Top Right
        ImageVertex(width, 0.0f - height, 0.0f, 1.0f, 1.0f)    // Bottom Right
    };
    D3DVERTEXELEMENT9 vertexElements[] = {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, sizeof(XMFLOAT3), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    hr = m_VertexBuffer.Init(vertices, ARRAYSIZE(vertices), vertexElements);
    if (FAILED(hr))
        return hr;

    m_Initialized = true;

    return hr;
}

HRESULT Image::InitShaders()
{
    HRESULT hr = S_OK;

    hr = ATG::LoadVertexShader("game:\\Media\\Shaders\\Image.xvu", &s_pVertexShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load vertex shader");
        return hr;
    }

    hr = ATG::LoadPixelShader("game:\\Media\\Shaders\\Image.xpu", &s_pPixelShader);
    if (FAILED(hr))
    {
        Log::Error("Couldn't load pixel shader");
        return hr;
    }

    s_ShadersInitialized = true;

    return hr;
}

void Image::CalculateWorldViewProjectionMatrix()
{
    // Direct3D uses an upwards Y axis system which is a bit unintuitive when dealing
    // with 2D rendering, so we flip the Y axis
    float x = static_cast<float>(m_Props.X);
    float y = static_cast<float>(m_Props.Y);
    m_WorldMatrix = XMMatrixTranslation(x, g_DisplayHeight - y, 0.0f);
    m_WVPMatrix = m_WorldMatrix * m_ViewMatrix * m_ProjectionMatrix;
}

void Image::UpdateVertexBuffer()
{
    // Create the vertices
    // Since the Y axis goes upwards, if we want a height increase to make our
    // rectangle grow downwards along the Y axis, we need to substract its height
    // to the Y coordinate of each vertex.
    float width = static_cast<float>(m_Props.Width);
    float height = static_cast<float>(m_Props.Height);
    ImageVertex vertices[] = {
        ImageVertex(0.0f, 0.0f - height, 0.0f, 0.0f, 1.0f),    // Bottom Left
        ImageVertex(0.0f, height - height, 0.0f, 0.0f, 0.0f),  // Top Left
        ImageVertex(width, height - height, 0.0f, 1.0f, 0.0f), // Top Right
        ImageVertex(width, 0.0f - height, 0.0f, 1.0f, 1.0f)    // Bottom Right
    };

    // Send the new vertices to the vertex buffer
    m_VertexBuffer.UpdateBuffer(vertices, ARRAYSIZE(vertices));
}

void Image::PopulateTexture()
{
    assert(m_pTexture != nullptr);
    assert(m_Props.pData != nullptr);
    assert(m_Props.Width > 0 && m_Props.Height > 0);

    D3DLOCKED_RECT rect = {};
    m_pTexture->LockRect(0, &rect, nullptr, 0);

    for (uint32_t y = 0; y < m_Props.Height; y++)
    {
        D3DCOLOR *pixels = (D3DCOLOR *)((uint8_t *)rect.pBits + y * rect.Pitch);
        memcpy(pixels, &m_Props.pData[y * m_Props.Width], m_Props.Width * sizeof(D3DCOLOR));
        pixels++;
    }

    m_pTexture->UnlockRect(0);
}

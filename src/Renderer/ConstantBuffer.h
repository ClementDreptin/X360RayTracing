#pragma once

template<typename T>
class ConstantBuffer
{
public:
    ConstantBuffer();

    ~ConstantBuffer();

    inline D3DConstantBuffer *operator&() const { return m_pBuffer; }

    HRESULT Init();

    void Update(const T *pData);

private:
    D3DConstantBuffer *m_pBuffer;
};

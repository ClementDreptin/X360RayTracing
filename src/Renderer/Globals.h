#pragma once

// The resolution is always 720p, 1080p is created by the hardware scaler
#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720
#define ASPECT_RATIO ((float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT)

// The texture resolution needs to be smaller than the display resolution, using
// actual 720p creates an out of memory error when creating the render target
#define MULTIPLIER 0.89
#define TEXTURE_WIDTH (uint32_t)(DISPLAY_WIDTH * MULTIPLIER)
#define TEXTURE_HEIGHT (uint32_t)(DISPLAY_HEIGHT * MULTIPLIER)

namespace ATG
{

extern D3DDevice *g_pd3dDevice;

}

using ATG::g_pd3dDevice;

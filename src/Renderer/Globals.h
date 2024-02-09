#pragma once

// The resolution is always 720p, 1080p is created by the hardware scaler
// NOTE: The resolution used is 720p * 0.9 to reduce the texture size, using
// actual 720p creates an out of memory error when creating the render target
#define DISPLAY_WIDTH 1152
#define DISPLAY_HEIGHT 648
#define ASPECT_RATIO ((float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT)

namespace ATG
{

extern D3DDevice *g_pd3dDevice;

}

using ATG::g_pd3dDevice;

#pragma once

#include <AtgDevice.h>

// The resolution is always 720p, 1080p is created by the hardware scaler
#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720
#define ASPECT_RATIO ((float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT)

namespace ATG
{

extern D3DDevice *g_pd3dDevice;

}

using ATG::g_pd3dDevice;

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

// The accumulation is stored in 32-bit floats, split across 2 render targets (RG and B)
// because EDRAM doesn't support 4 channel 32-bit float formats. That's 128 bits per pixel,
// which doesn't fit in EDRAM at the texture resolution, so the image is rendered in
// horizontal tiles
#define ACCUMULATION_TARGET_COUNT 2
#define TILE_COUNT 4
#define TILE_HEIGHT (TEXTURE_HEIGHT / TILE_COUNT)

#define RANDOM_TEXTURE_SIZE 256
#define BOUNCES 3

namespace ATG
{

extern D3DDevice *g_pd3dDevice;

}

using ATG::g_pd3dDevice;

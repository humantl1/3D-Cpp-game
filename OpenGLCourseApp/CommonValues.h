#ifndef COMMONVALS
#define COMMONVALS

#include "stb_image.h"

const int MAX_POINT_LIGHTS = 3; // Should always match value in shader.frag
const int MAX_SPOT_LIGHTS = 3;  // Should always match value in shader.frag

const unsigned int kFPS = 60;
const float kFrameTargetTime = 1000.0f / kFPS;
const float kMaxDeltaTime = .05f; // 20 fps

#endif

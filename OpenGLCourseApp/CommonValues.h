#ifndef COMMONVALS
#define COMMONVALS

#include "stb_image.h"

const float kToRadians = 3.14159265f / 180.0f;

const int kMaxPointLights = 3; // Should always match value in shader.frag
const int kMaxSpotLights = 3;  // Should always match value in shader.frag

const unsigned int kFPS = 60;
const float kFrameTargetTime = 1000.0f / kFPS;
const float kMaxDeltaTime = .05f; // 20 fps

#endif

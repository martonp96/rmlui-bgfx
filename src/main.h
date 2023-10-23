#pragma once

//windows & std
#define NOMINMAX
#include <Windows.h>
#include <functional>
#include <optional>
#include <chrono>
#include <fstream>
#include <algorithm>

//bgfx
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <bx/timer.h>

//eigen
#include <Eigen/Eigen>

//rmlui
#include <RmlUi/Core.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/StringUtilities.h>
#include <RmlUi/Core/SystemInterface.h>
#include <RmlUi/Core/StreamMemory.h>
#include <RmlUi/Core/TransformPrimitive.h>
#include <RmlUi/Debugger.h>

//stb
#include <stb/stb_image.h>

//sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
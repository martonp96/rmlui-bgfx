#pragma once

//std
#include <functional>
#include <optional>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <mutex>

//bgfx
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <bx/thread.h>
#include <bx/spscqueue.h>

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

//sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

//spdlog
#include <spdlog/spdlog.h>

namespace window
{
    class CEvent;
}

namespace core
{
    class CLogger;
}
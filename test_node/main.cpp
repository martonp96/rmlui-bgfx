#define NODE_ADDON_API_DISABLE_DEPRECATED
#include <napi.h>

#include "api/api_base.h"
#include "napi_type_helpers.h"
#include "napi_binding_helpers.h"
#include "napi_bindings.h"

#include <cstdio>
#include <string>

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    AppWrapper::Init(env, exports);
    ElemWrapper::Init(env, exports);
    DocWrapper::Init(env, exports);
    return exports;
}

NODE_API_MODULE(voidRML, InitAll)

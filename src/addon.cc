#include <napi.h>

#include "Table.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    Table::Init(env, exports);

    return exports;
}

NODE_API_MODULE(addon, InitAll)

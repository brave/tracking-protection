/* Copyright (c) 2016 Sergiy Zhukovs'kyy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <node.h>
#include "TPParserWrap.h"

namespace {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
  TPParserWrap::CTPParserWrap::Init(exports);
}

void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Value> unused, void* priv) {
  InitAll(exports);
}

void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context, void* priv) {
  InitAll(exports);
}

}  // namespace

#ifdef TP_INTERNAL_MODULE
NODE_MODULE_CONTEXT_AWARE_BUILTIN(tp_node_addon, InitAll)
#else
NODE_MODULE(tp_node_addon, InitAll)
#endif

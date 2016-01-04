#include <node.h>
#include <node_buffer.h>
#include "../TPParser.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Function;
using v8::Value;
using v8::Exception;

CTPParser gParser;
char* deserializedData = nullptr;

void addTracker(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }
    
    String::Utf8Value str(args[0]->ToString());
    
    const char * buffer = *str;
    
    gParser.addTracker(buffer);
}

void matchesTracker(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }
    
    String::Utf8Value str(args[0]->ToString());
    
    const char * buffer = *str;
    
    args.GetReturnValue().Set(gParser.matchesTracker(buffer));
}

void addFirstPartyHosts(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    if (!args[0]->IsString() || !args[1]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }
    
    String::Utf8Value strFirstHost(args[0]->ToString());
    String::Utf8Value strThirdPartyHosts(args[1]->ToString());
    
    const char* firstHost = *strFirstHost;
    const char* thirdPartyHosts = *strThirdPartyHosts;
    
    gParser.addFirstPartyHosts(firstHost, thirdPartyHosts);
}

void findFirstPartyHosts(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }
    
    String::Utf8Value strFirstHost(args[0]->ToString());
    
    const char* firstHost = *strFirstHost;
    
    char* thirdPartyHosts = gParser.findFirstPartyHosts(firstHost);
    if (nullptr != thirdPartyHosts) {
        v8::Local<String> toReturn = String::NewFromUtf8(isolate, thirdPartyHosts);
        args.GetReturnValue().Set(toReturn);
        delete []thirdPartyHosts;
    }
}

void serialize(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    unsigned int totalSize = 0;
    // Serialize data
    char* data = gParser.serialize(&totalSize);
    if (nullptr == data) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Could not serialize")));
        return;
    }
    
    MaybeLocal<Object> buffer = node::Buffer::New(isolate, totalSize);
    Local<Object> localBuffer;
    if (!buffer.ToLocal(&localBuffer)) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Could not convert MaybeLocal to Local")));
        return;
    }
    ::memcpy(node::Buffer::Data(localBuffer), data, totalSize);
    
    delete []data;
    
    args.GetReturnValue().Set(localBuffer);
}

void deserialize(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(
                                                     String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    
    unsigned char *buf = (unsigned char *)node::Buffer::Data(args[0]);
    size_t length = node::Buffer::Length(args[0]);
    
    if (nullptr != deserializedData) {
        delete []deserializedData;
    }
    deserializedData = new char[length];
    memcpy(deserializedData, buf, length);
    
    gParser.deserialize(deserializedData);
}

void cleanup(const FunctionCallbackInfo<Value>&) {
    if (nullptr != deserializedData) {
        delete []deserializedData;
        deserializedData = nullptr;
    }
}


void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "addTracker", addTracker);
    NODE_SET_METHOD(exports, "matchesTracker", matchesTracker);
    NODE_SET_METHOD(exports, "addFirstPartyHosts", addFirstPartyHosts);
    NODE_SET_METHOD(exports, "findFirstPartyHosts", findFirstPartyHosts);
    NODE_SET_METHOD(exports, "serialize", serialize);
    NODE_SET_METHOD(exports, "deserialize", deserialize);
    NODE_SET_METHOD(exports, "cleanup", cleanup);
}

NODE_MODULE(tp_node_addon, init)
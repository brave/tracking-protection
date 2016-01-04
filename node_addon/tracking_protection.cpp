#include <node.h>
#include "../TPParser.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Function;
using v8::Value;
using v8::Exception;

CTPParser gParser;

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


void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "addTracker", addTracker);
    NODE_SET_METHOD(exports, "matchesTracker", matchesTracker);
    NODE_SET_METHOD(exports, "addFirstPartyHosts", addFirstPartyHosts);
    NODE_SET_METHOD(exports, "findFirstPartyHosts", findFirstPartyHosts);
}

NODE_MODULE(tp_node_addon, init)
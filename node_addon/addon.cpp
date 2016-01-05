#include <node.h>
#include "TPParserWrap.h"

namespace TPParserWrap {
    
    using v8::Local;
    using v8::Object;
    
    void InitAll(Local<Object> exports) {
        CTPParserWrap::Init(exports);
    }
    
    NODE_MODULE(tp_node_addon, InitAll)
    
}  // namespace TPParserWrap
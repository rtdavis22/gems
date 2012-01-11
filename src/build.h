#ifndef GEMS_BUILD_H_
#define GEMS_BUILD_H_

#include "v8.h"

namespace gems {

class GemsStructure;

struct Build {
    static v8::Handle<v8::Value> JSBuild(const v8::Arguments& args);

    GemsStructure *operator()(v8::Handle<v8::Value> value);

    GemsStructure *operator()(v8::Handle<v8::String> v8_string);

    GemsStructure *operator()(v8::Array *array);
    GemsStructure *operator()(v8::Handle<v8::Object> object);
};

}  // namespace gems

#endif  // GEMS_BUILD_H_

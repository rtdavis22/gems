#ifndef GEMS_BUILD_H_
#define GEMS_BUILD_H_

#include "v8.h"

namespace gmml {

class Structure;

}  // namespace gmml

namespace gems {

struct Build {
    static v8::Handle<v8::Value> JSBuild(const v8::Arguments& args);

    gmml::Structure *operator()(v8::Handle<v8::Value> value);

    gmml::Structure *operator()(v8::Array *array);
    gmml::Structure *operator()(v8::Handle<v8::Object> object);
};

}  // namespace gems

#endif  // GEMS_BUILD_H_

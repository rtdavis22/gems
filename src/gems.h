// Author: Robert Davis

#ifndef GEMS_GEMS_H_
#define GEMS_GEMS_H_

#include "v8.h"

namespace gems {

class Gems {
  public:
    static void Init(v8::Handle<v8::ObjectTemplate> object_template);

    static v8::Handle<v8::Value> BuildGlycam(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddPath(const v8::Arguments& args);

    static v8::Handle<v8::Value> AddResidueMapping(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddHeadMapping(const v8::Arguments& args);
    static v8::Handle<v8::Value> AddTailMapping(const v8::Arguments& args);

    static v8::Handle<v8::Value> Measure(const v8::Arguments& args);
};

struct Object {
  virtual ~Object() {}
};

}  // namespace gems

#endif  // GEMS_GEMS_H_

// Author: Robert Davis

#ifndef GEMS_STRUCTURE_H_
#define GEMS_STRUCTURE_H_

#include "v8.h"

namespace gmml {

class Structure;

}

namespace gems {

class StructureModule {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);

    static gmml::Structure *extract_ptr(v8::Handle<v8::Value> value);
};

}  // namespace gems

#endif  // GEMS_STRUCTURE_H_

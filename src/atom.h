// Author: Robert Davis

#ifndef GEMS_ATOM_H_
#define GEMS_ATOM_H_

#include "v8.h"

namespace gems {

class AtomModule {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);
};

}  // namespace gems

#endif  // GEMS_ATOM_H_

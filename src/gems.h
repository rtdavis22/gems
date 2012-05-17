// Author: Robert Davis

#ifndef GEMS_GEMS_H_
#define GEMS_GEMS_H_

#include <string>

#include "v8.h"

namespace gems {

class Gems {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);
    static void load_files();
};

}  // namespace gems

#endif  // GEMS_GEMS_H_

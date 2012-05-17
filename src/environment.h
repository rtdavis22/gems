// Author: Robert Davis

#ifndef GEMS_ENVIRONMENT_H_
#define GEMS_ENVIRONMENT_H_

#include "v8.h"

namespace gems {

class EnvironmentModule {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);
};

}  // namespace gems

#endif  // GEMS_ENVIRONMENT_H_

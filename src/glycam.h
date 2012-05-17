// Author: Robert Davis

#ifndef GEMS_GLYCAM_H_
#define GEMS_GLYCAM_H_

#include "v8.h"

namespace gems {

class GlycamModule {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);
};

}  // namespace gems

#endif  // GEMS_GLYCAM_H_

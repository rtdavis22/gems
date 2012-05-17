// Author: Robert Davis

#ifndef GEMS_PDB_H_
#define GEMS_PDB_H_

#include "v8.h"

namespace gems {

class PdbModule {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);
};

}  // namespace gems

#endif  // GEMS_PDB_H_

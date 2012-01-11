// Author: Robert Davis

#ifndef GEMS_RESIDUE_H_
#define GEMS_RESIDUE_H_

#include "v8.h"

#include "gems.h"

namespace gmml {

class Residue;
class Structure;

}  // namespace gmml

namespace gems {

struct GemsResidue : public Object {
    GemsResidue(gmml::Structure *structure, int index) : structure(structure),
                                                         index(index) {}

    gmml::Residue *residue();

    gmml::Structure *structure;
    int index;
};

class ResidueWrapper {
  public:
    static v8::Handle<v8::Object> wrap(GemsResidue *residue);

  private:
    static void callback(v8::Persistent<v8::Value> object, void *data);
};

}  // namespace gems

#endif  // GEMS_RESIDUE_H_

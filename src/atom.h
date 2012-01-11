// Author: Robert Davis

#ifndef GEMS_ATOM_H_
#define GEMS_ATOM_H_

#include "v8.h"

#include "gems.h"

// External forward-declarations
namespace gmml {

class Atom;
class Structure;

}  // namespace gmml

namespace gems {

// Should wrap() go in here?
struct GemsAtom : public Object {
    GemsAtom(gmml::Structure *structure, int index) : structure(structure),
                                                      index(index) {}

    gmml::Atom *atom();

    // Should this be a GemsStructure instead?
    gmml::Structure *structure;
    int index;
};

class AtomWrapper {
  public:
    static v8::Handle<v8::Object> wrap(GemsAtom *atom);

  private:
    static void callback(v8::Persistent<v8::Value> object, void *data);
};

}  // namespace gems

#endif  // GEMS_ATOM_H_

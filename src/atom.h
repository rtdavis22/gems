// Author: Robert Davis

#ifndef GEMS_ATOM_H_
#define GEMS_ATOM_H_

#include "v8.h"

#include "boost/shared_ptr.hpp"

// External forward-declarations
namespace gmml {

class Atom;
class Structure;

}  // namespace gmml

namespace gems {

class AtomWrapper {
  public:
    // The parameter should have type Structure::AtomPtr, but unfortunately
    // you can't forward declare a typedef within a class.
    static v8::Handle<v8::Object> wrap(boost::shared_ptr<gmml::Atom> atom);

  private:
    static void callback(v8::Persistent<v8::Value> object, void *data);
};

}  // namespace gems

#endif  // GEMS_ATOM_H_

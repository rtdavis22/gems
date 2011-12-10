// Author: Robert Davis

#ifndef GEMS_STRUCTURE_H_
#define GEMS_STRUCTURE_H_

#include "v8.h"

namespace gmml {

class Structure;

}

namespace gems {

v8::Handle<v8::Value> StructureConstructor(const v8::Arguments& args);

class StructureWrapper {
  public:
    // Returns the javascript representation of the structure.
    static v8::Handle<v8::Object> wrap(gmml::Structure *structure);

    // This function frees memory that's allocated lazily by wrap().
    // Since the wrapper will likely be used until the end of the program, it
    // probably isn't necessary to clean up this memory, but this is
    // included for thoroughness. 
    static void destroy();

  private:
    // This function is called by the garbage collector when only weak handles
    // refer to the object.
    static void callback(v8::Persistent<v8::Value> object, void *data);
};

}  // namespace gems

#endif  // GEMS_STRUCTURE_H_

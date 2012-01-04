#include "coordinate.h"

#include "gmml/gmml.h"

#include "atom.h"
#include "gems.h"

using gmml::Coordinate;

using v8::External;
using v8::Local;

namespace gems {

Coordinate *ExtractCoordinate::operator()(v8::Handle<v8::Value> value) {



    Local<v8::Object> object = value->ToObject();
    Local<External> wrap = Local<External>::Cast(object->GetInternalField(0));
    gems::Object *gems_object = static_cast<gems::Object*>(wrap->Value());
    GemsAtom *atom = dynamic_cast<GemsAtom*>(gems_object);
    if (atom == NULL)
       return NULL;
    return new Coordinate(atom->atom()->coordinate());
}

Coordinate *ExtractCoordinate::operator()(Object *object,
                                          v8::Handle<v8::Value> value) {
    return NULL;
}

}  // namespace gems

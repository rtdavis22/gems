#ifndef GEMS_EXTRACT_COORDINATE_H_
#define GEMS_EXTRACT_COORDINATE_H_

#include "v8.h"

namespace gmml {

struct Coordinate;

}  // namespace gmml

namespace gems {

struct Object;

struct ExtractCoordinate {
    gmml::Coordinate *operator()(v8::Handle<v8::Value> value);

    gmml::Coordinate *operator()(Object *object, v8::Handle<v8::Value> value);
};

}  // namespace gems

#endif  // GEMS_EXTRACT_COORDINATE_H_

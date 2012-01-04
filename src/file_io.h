#ifndef GEMS_FILE_IO_H_
#define GEMS_FILE_IO_H_

#include "v8.h"

namespace gems {

struct FileIO {
    static v8::Handle<v8::Value> LoadLibraryFile(const v8::Arguments& args);
    static v8::Handle<v8::Value> LoadParameterFile(const v8::Arguments& args);
    static v8::Handle<v8::Value> LoadPrepFile(const v8::Arguments& args);
};

}  // namespace gems

#endif  // GEMS_FILE_IO_H_

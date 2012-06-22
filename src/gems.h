// Copyright (c) 2012 The University of Georgia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Author: Robert Davis

#ifndef GEMS_GEMS_H_
#define GEMS_GEMS_H_

#include <string>

#include "v8.h"

namespace gems {

class Gems {
  public:
    static void init(v8::Handle<v8::ObjectTemplate> object_template);
    static void load_files();

    static void dont_load_glycam() { kIsGlycamLoaded = false; }

  private:
    static v8::Handle<v8::Value> is_glycam_loaded(v8::Local<v8::String>,
                                                  const v8::AccessorInfo&) {
        return v8::Boolean::New(kIsGlycamLoaded);
    }

    static bool kIsGlycamLoaded;
};

}  // namespace gems

#endif  // GEMS_GEMS_H_

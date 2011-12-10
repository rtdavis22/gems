#! /bin/sh

# Run this script to generate the configure script and other files that will
# be included in the distribution.  These files are not checked in because they
# are automatically generated.

set -e

if test ! -e deps/v8; then
  echo "Fetching v8 from the web..."
  svn export http://v8.googlecode.com/svn/trunk/ deps/v8
fi

if test ! -e build/gyp; then
  echo "Fetching gyp from the web..."
  svn export http://gyp.googlecode.com/svn/trunk/ build/gyp
fi

set -ex

autoreconf -f -i

rm -rf autom4te.cache config.h.in~
exit 0

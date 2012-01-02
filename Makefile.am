BUILDTYPE ?= Release

SUBDIRS = deps/gmml

all-local: out/Makefile
	$(MAKE) -C out BUILDTYPE=$(BUILDTYPE)
	-ln -fs out/$(BUILDTYPE)/gems gems

build/options.gypi:
	gyp_configure

out/Makefile: build/gems.gyp build/common.gypi build/options.gypi
	build/run_gyp

clean-local:
	rm -f gems
	rm -rf out

dist-hook:
	svn export http://v8.googlecode.com/svn/trunk/ $(distdir)/deps/v8
	svn export http://gyp.googlecode.com/svn/trunk/ $(distdir)/build/gyp

EXTRA_DIST = \
  autogen.sh \
  build/common.gypi \
  build/gems.gyp \
  build/run_gyp \
  gyp_configure \
  src/atom.cc \
  src/atom.h \
  src/element.cc \
  src/element.h \
  src/gems.cc \
  src/gems.h \
  src/pdb_structure.cc \
  src/pdb_structure.h \
  src/shell.cc \
  src/structure.cc \
  src/structure.h

MAINTAINERCLEANFILES = \
  aclocal.m4 \
  config.h \
  config.h.in \
  config.log \
  config.status \
  configure depcomp \
  INSTALL \
  install-sh \
  Makefile \
  Makefile.in \
  missing stamp-h1 \
  config.guess \
  config.sub \
  ltmain.sh \
  libtool \
  config.h~ \
  m4/ltsugar.m4 \
  m4/libtool.m4 \
  m4/ltversion.m4 \
  m4/lt~obsolete.m4 \
  m4/ltoptions.m4

maintainer-clean-local: clean-local
	rm -f $(MAINTAINERCLEANFILES)
	rm -rf autom4te.cache
        

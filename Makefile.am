BUILDTYPE ?= Release

SUBDIRS = deps/gmml

PREFIX = /usr/local

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

install: all-local
	install -m 0755 -d $(PREFIX)/bin
	install -m 0755 gems $(PREFIX)/bin/gems
	for f in $$(find src/js/ dat -type f); \
	do \
	    DIR=`dirname $$f` || exit 1; \
	    install -m 0755 -d $(PREFIX)/share/gems/$$DIR || exit 1; \
	    install -m 0644 $$f $(PREFIX)/share/gems/$$f || exit 1; \
	done

EXTRA_DIST = \
  autogen.sh \
  build/common.gypi \
  build/gems.gyp \
  build/run_gyp \
  gyp_configure \
  src/js/atom.js \
  src/js/core.js \
  src/js/environment.js \
  src/js/glycam.js \
  src/js/pdb.js \
  src/js/structure.js \
  src/js/utils.js \
  src/atom_module.cc \
  src/atom_module.h \
  src/environment_module.cc \
  src/environment_module.h \
  src/gems.cc \
  src/gems.h \
  src/glycam_module.cc \
  src/glycam_module.h \
  src/pdb_module.cc \
  src/pdb_module.h \
  src/shell.cc \
  src/shell.h \
  src/structure_module.cc \
  src/structure_module.h


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
        

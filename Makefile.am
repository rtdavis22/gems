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
  src/gems.h \
  src/shell.cc \
  src/residue_module.h \
  src/glycam_module.cc \
  src/environment_module.h \
  src/prep_file_module.h \
  src/glycam_module.h \
  src/gems.cc \
  src/environment_module.cc \
  src/residue_module.cc \
  src/pdb_module.h \
  src/atom_module.cc \
  src/pdb_module.cc \
  src/prep_file_module.cc \
  src/js/residue_module.js \
  src/js/environment_module.js \
  src/js/glycam_module.js \
  src/js/pdb_module.js \
  src/js/structure_module.js \
  src/js/prep_file_module.js \
  src/js/core.js \
  src/js/atom_module.js \
  src/js/utils.js \
  src/shell.h \
  src/structure_module.h \
  src/structure_module.cc \
  src/atom_module.h \
  examples/geometry.js \
  examples/robert.js \
  examples/solvate.js \
  examples/structure.js \
  examples/tip3pbox.off \
  examples/residues.js \
  examples/load_pdb.js \
  examples/build.js \
  examples/1RVZ.pdb \
  examples/write_pdb.js \
  examples/glycam_sequence.js \
  examples/ff99SB.js \
  examples/glycoprotein.js \
  dat/all_aminoct94.lib \
  dat/GLYCAM_amino_06h.lib \
  dat/all_amino94.lib \
  dat/parm99.dat \
  dat/all_aminont94.lib \
  dat/GLYCAM_aminont_06h.lib \
  dat/HOH.prep \
  dat/GLYCAM_06h.dat \
  dat/Glycam_06.prep \
  dat/GLYCAM_aminoct_06h.lib \
  dat/GLYCAM_06h.prep



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
        

SUBDIRS = deps/gmml #.

#bin_PROGRAMS = gems

#gems_SOURCES = \
#  src/atom.cc \
#  src/atom.h \
#  src/gems.cc \
#  src/gems.h \
#  src/shell.cc \
#  src/structure.cc \
#  src/structure.h

#gems_CPPFLAGS = \
#  -I$(top_srcdir)/v8/include \
#  -I$(top_srcdir)/gmml/include

#gems_LDFLAGS = -lpthread

#gems_LDADD = \
#  $(top_srcdir)/v8/libv8.a \
#  $(top_srcdir)/gmml/libgmml.a

all-local: out/Makefile
	$(MAKE) -C out
	-ln -fs out/Debug/gems gems

out/Makefile: gems.gyp common.gypi options.gypi
	tools/run_gyp

clean-local:
	rm -f gems
	rm -rf out

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

SUBDIRS = deps/gmml

all-local: out/Makefile
	$(MAKE) -C out
	-ln -fs out/Debug/gems gems

out/Makefile: build/gems.gyp build/common.gypi build/options.gypi
	build/run_gyp

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

maintainer-clean-local:
	rm -f $(MAINTAINERCLEANFILES)
	rm -rf autom4te.cache

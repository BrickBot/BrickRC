## Process this file with automake to produce Makefile.in

SUBDIRS = macros src

EXTRA_DIST = \
	autogen.sh \
	brickRC.glade \
	rcx

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE); \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/$(PACKAGE); \
	    fi \
	  done \
	fi 
	$(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE)
	$(INSTALL_DATA) $(PACKAGE).glade $(DESTDIR)$(datadir)/$(PACKAGE)

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi


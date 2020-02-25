## Process this file with automake to produce Makefile.in

INCLUDES = $(GNOME_INCLUDEDIR) $(LIBGLADE_CFLAGS) $(IMLIB_CFLAGS) -I/opt/gnome/include \
	-DDATADIR=\"$(pkgdatadir)\"

bin_PROGRAMS = brickrc

brickrc_SOURCES = \
	main.c main.h \
	motor.c motor.h \
	sensor.c sensor.h \
	rcx.c rcx.h 
brickrc_LDADD = $(GNOME_LIBDIR) $(GNOMEUI_LIBS) $(LIBGLADE_LIBS) -llnp


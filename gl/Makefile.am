include $(top_srcdir)/config/common.am

if ENABLE_XFT
INCLUDES = -DMAKING_FORMS -I../lib -I$(top_srcdir)/lib $(X_CFLAGS) $$(pkg-config --cflags freetype2)
else
INCLUDES = -DMAKING_FORMS -I../lib -I$(top_srcdir)/lib $(X_CFLAGS)
endif

include_HEADERS = glcanvas.h

lib_LTLIBRARIES = libformsGL.la

libformsGL_la_LDFLAGS = -no-undefined -version-info @SO_VERSION@

if ENABLE_XFT
libformsGL_la_LIBADD = ../lib/libforms.la $(X_LIBS) -lGL -lX11 -lXft
else
libformsGL_la_LIBADD = ../lib/libforms.la $(X_LIBS) -lGL -lX11
endif

libformsGL_la_SOURCES = \
	glcanvas.c \
	glcanvas.h

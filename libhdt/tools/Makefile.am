bin_PROGRAMS = hdt2rdf hdtInfo hdtSearch modifyHeader rdf2hdt replaceHeader searchHeader benchmark-hdt translatestring2id hdt2hdtpp hdtpp2rdf hdtppSearch

AM_DEFAULT_SOURCE_EXT = .cpp

AM_CPPFLAGS = -I@top_srcdir@/libhdt/include $(WARN_CFLAGS) $(EXTRAFLAGS) -I@top_srcdir@/libsdsl/include -L@top_srcdir@/libsdsl/lib -lsdsl -ldivsufsort -ldivsufsort64 
AM_LDFLAGS = $(SERD_LIBS) $(ZLIB_LIBS) $(KYOTO_LIBS)
LDADD = ../libhdt.la
LDADD += $(top_builddir)/libsdsl/lib/libsdsl.a

if WANTS_LIBCDS
AM_CPPFLAGS += -DHAVE_CDS -I$(top_builddir)/libcds/include
LDADD += $(top_builddir)/libcds/libcds.la
endif



# $Id$

noinst_LTLIBRARIES = libutil.la
libutil_la_SOURCES = exceptions.hh \
		     string.hh string.cc \
		     misc.hh misc.cc \
		     regex.hh regex.cc \
		     file.hh file.cc \
		     vars.hh vars.cc \
		     cache.hh \
		     input.hh input.cc

LIBS = @LIBS@ @READLINE_LIBS@
AM_CXXFLAGS = -pedantic -ansi -Wall

MAINTAINERCLEANFILES = Makefile.in *.loT *~

srchtml:
	@source-highlight -c ../src.css -s cpp -f html -t4 -n -i *.{cc,hh} \
		--output-dir=../html/util

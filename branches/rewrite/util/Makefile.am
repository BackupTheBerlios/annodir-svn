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

srchtml: $(foreach f, $(libutil_la_SOURCES), $(f).html)
%.cc.html: %.cc
	@echo Generating $@
	@source-highlight -c ../src.css -s cpp -f html -t4 -n -i $? -o ../html/util/$@
%.hh.html: %.hh
	@echo Generating $@
	@source-highlight -c ../src.css -s cpp -f html -t4 -n -i $? -o ../html/util/$@

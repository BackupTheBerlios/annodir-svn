# $Id$

bin_PROGRAMS = annodir
annodir_SOURCES = common.hh \
		  exceptions.hh \
		  options.hh options.cc \
		  action_handler.hh \
		  action_list_handler.hh action_list_handler.cc \
		  annodir.cc
annodir_LDADD = ../util/libutil.la


AM_CXXFLAGS = -pedantic -ansi -Wall

MAINTAINERCLEANFILES = Makefile.in *~

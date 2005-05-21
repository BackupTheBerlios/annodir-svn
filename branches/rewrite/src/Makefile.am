# $Id$

bin_PROGRAMS = annodir
annodir_SOURCES = common.hh common.cc \
		  exceptions.hh \
		  options.hh options.cc \
		  db.hh db.cc \
		  db_entry.hh db_entry.cc \
		  db_note_entry.hh db_note_entry.cc \
		  db_meta_entry.hh db_meta_entry.cc \
		  db_link_entry.hh db_link_entry.cc \
		  action_handler.hh \
		  action_list_handler.hh action_list_handler.cc \
		  action_add_handler.hh action_add_handler.cc \
		  action_delete_handler.hh action_delete_handler.cc \
		  action_edit_handler.hh action_edit_handler.cc \
		  action_export_handler.hh action_export_handler.cc \
		  annodir.cc
annodir_LDADD = ../util/libutil.la

AM_CXXFLAGS = -pedantic -ansi -Wall

MAINTAINERCLEANFILES = Makefile.in *~

srchtml:
	@source-highlight -c ../src.css -s cpp -f html -t4 -n -i *.{cc,hh} \
		--output-dir=../html/src

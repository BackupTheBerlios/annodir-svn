bin_PROGRAMS = annodir

annodir_SOURCES = \
		  exceptions.hh \
		  action_handler.hh \
		  util.hh util.cc \
		  rcfile.hh rcfile.cc \
		  input.hh input.cc \
		  action_list_handler.hh action_list_handler.cc \
		  action_add_handler.hh action_add_handler.cc \
		  action_export_handler.hh action_export_handler.cc \
		  options.hh options.cc \
		  database.hh database.cc \
		  database_entry.hh database_entry.cc \
		  database_note_entry.hh database_note_entry.cc \
		  database_link_entry.hh database_link_entry.cc \
		  database_metadata_entry.hh database_metadata_entry.cc \
		  annodir.cc

LIBS = @READLINE_LIBS@

AM_CXXFLAGS = -Wall
MAINTAINERCLEANFILES = Makefile.in *~

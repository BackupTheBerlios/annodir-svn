/*
 * annodir -- src/action_delete_handler.cc
 * $Id: node_entry.hh 92 2004-12-10 11:35:38Z ka0ttic $
 * Copyright (c) 2004 Ciaran McCreesh <ciaranm at gentoo.org>
 * Copyright (c) 2004 Aaron Walker <ka0ttic at gentoo.org>
 *
 * This file is part of annodir.
 *
 * annodir is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * annodir is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * annodir; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 325, Boston, MA  02111-1257  USA
 */

#include "config.h"
#include <fstream>
#include <string>
#include <memory>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>	/* for unlink() */

#include "src/action_delete_handler.hh"
#include "src/node_entry.hh"
#include "src/database.hh"
#include "src/exceptions.hh"
#include "src/input.hh"
#include "src/options.hh"

    int
action_delete_handler_T::operator() (void)
{
    options_T options;

    try
    {
	/* load database */
	std::auto_ptr<database_T > db(new database_T());

	{
	    std::auto_ptr<std::istream > f(new
		    std::ifstream(options.get_filename().c_str()));
	    if ((*f))
		db->load(*f);
	    else
	    {
		if (ENOENT != errno)
		    throw annodir_file_unreadable_E();
	    }
	}

	/* index not specified, prompt for one */
	if (options.get_index().empty())
	{
	    char *in = input::get_user_input("Index");
	    if (in)
	    {
		std::string s(in);
		free(in);
		options.set_index(s);
	    }
	    else
		throw node_invalid_index_E();
	}

	/* find the index */
	node_entry_T *node = db->find_index(options.get_index());
	if (not node)
	    throw node_invalid_index_E();

	/* we need to call erase() from the parent */
	node_entry_T *parent = node->parent();

	node_entry_T::iterator i;
	for (i = parent->begin(); i != parent->end() ; i++)
	{
	    if ((*i)->index() == node->index())
		break;
	}
	parent->erase(i);
	node = NULL;

	/* save */
	{
	    std::auto_ptr<std::ostream > f(new
		std::ofstream(options.get_filename().c_str()));
	    if ((*f))
		db->dump(*f);
	    else
		throw annodir_file_unwriteable_E();
	}

	if (db->empty() and options.delete_on_empty())
	{
	    if (0 != unlink(options.get_filename().c_str()))
		throw annodir_file_unwriteable_E();
	}
    }
    catch (annodir_file_unreadable_E)
    {
        /* bleh! file there, can't read it. */
        std::cout << "Error: couldn't open " << options.get_filename()
            << " for read (" << errno << "): " << strerror(errno) 
            << std::endl;
        return EXIT_FAILURE;
    }
    catch (annodir_file_unwriteable_E)
    {
        /* bleh! file there, can't read it. */
        std::cout << "Error: couldn't open " << options.get_filename()
            << " for write (" << errno << "): " << strerror(errno) 
            << std::endl;
        return EXIT_FAILURE;
    }
    catch (node_invalid_index_E)
    {
        std::cout << "Error: ";
	if (options.get_index().empty())
	    std::cout << "You must specify an index.";
	else
	    std::cout << "Invalid index '" << options.get_index() << "'.";
	std::cout << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/*
 * annodir -- src/action_add_handler.cc
 * Copyright (c) 2004 Ciaran McCreesh <ciaranm at gentoo.org>
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
#include <errno.h>
#include <cstdlib>
#include <memory>
#include <fstream>

#include "src/database.hh"
#include "src/exceptions.hh"
#include "src/options.hh"
#include "src/action_add_handler.hh"
#include "src/database_note_entry.hh"

    int
action_add_handler_T::operator() (void)
{
    options_T options;

    try
    {
        /* load database */
        std::auto_ptr<database_T > db(new database_T());

        std::auto_ptr<std::istream > f(new 
                std::ifstream(options.get_filename().c_str()));
        if ((*f))
            db->load(*f);
        else
        {
            if (ENOENT != errno)
                throw annodir_file_unreadable_E();
        }

        /* add a new entry */
        database_entry_T *entry = new database_note_entry_T;
        entry->set_new_object_defaults();
        if (entry->prompt_user_for_values())
        {
            db->entries.push_back(entry);

            /* dump to stdout for now */
            db->dump(std::cout);
        }
        else
            return EXIT_FAILURE;

    }
    catch (annodir_file_unreadable_E)
    {
        /* bleh! file there, can't read it. */
        std::cout << "Error: couldn't open " << options.get_filename()
            << " for read (" << errno << "): " << strerror(errno) 
            << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

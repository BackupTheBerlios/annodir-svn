/*
 * annodir -- src/action_add_handler.cc
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
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <fstream>

#include "src/database.hh"
#include "src/exceptions.hh"
#include "src/options.hh"
#include "src/action_add_handler.hh"
#include "src/database_note_entry.hh"
#include "src/database_link_entry.hh"
#include "src/input.hh"

    database_entry_T *
make_new_entry(const char *type)
{
    if (0 == strcasecmp(type, "prompt"))
    {
        char *in = get_user_input("Item Type");
        if (in)
        {
            return make_new_entry(in);
            free(in);
        }
        else
            return NULL;
    }
    else if (0 == strcasecmp(type, "note"))
        return new database_note_entry_T;
    else if (0 == strcasecmp(type, "link"))
        return new database_link_entry_T;
    else /* fallback */
        return new database_entry_T; 
}

    int
action_add_handler_T::operator() (void)
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

        /* add a new entry */
        database_entry_T *entry = make_new_entry(
                options.get_type().c_str());

        if (!entry)
            return EXIT_FAILURE;

        entry->set_new_object_defaults();
        if (entry->prompt_user_for_values())
        {
            db->entries.push_back(entry);

            /* save */
            std::auto_ptr<std::ostream> f(new
                    std::ofstream(options.get_filename().c_str()));
            if ((*f))
                db->dump(*f);
            else
                throw annodir_file_unwriteable_E();
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
    catch (annodir_file_unwriteable_E)
    {
        /* bleh! file there, can't read it. */
        std::cout << "Error: couldn't open " << options.get_filename()
            << " for write (" << errno << "): " << strerror(errno) 
            << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

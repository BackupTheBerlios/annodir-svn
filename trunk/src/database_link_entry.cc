/*
 * annodir -- src/database_link_entry.cc
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
#include "src/database_link_entry.hh"
#include "src/exceptions.hh"
#include "src/database.hh"
#include "src/options.hh"
#include "src/input.hh"

#include <errno.h>
#include <cstdlib>
#include <memory>
#include <fstream>

/*
 * Create a new item read from the supplied stream.
 */
database_link_entry_T::database_link_entry_T(std::istream *stream)
    : database_entry_T(stream)
{
    id = default_id();
}

/*
 * Get the default id for an object of our kind
 */
    std::string
database_link_entry_T::default_id()
{
    return "link";
}

/*
 * Do we recognise this item?
 */
    bool
database_link_entry_T::recognise_item(std::string item)
{
    return (item == "link");
}

    void
database_link_entry_T::set_new_object_defaults()
{
    database_entry_T::set_new_object_defaults();
}

    bool
database_link_entry_T::prompt_user_for_values()
{
    if (!database_entry_T::prompt_user_for_values())
        return false;

    char *input = NULL;
    if (!((input = get_user_input("Location"))))
        return false;
    keys["location"].assign(input);

    return true;
}

/*
 * open "link" and display
 */
    void
database_link_entry_T::display(std::ostream &stream)
{
    options_T options;

    try
    {
        std::auto_ptr<std::ifstream > f(new
                std::ifstream(keys["location"].c_str()));
        if (! (*f))
        {
            if (ENOENT == errno)
                throw annodir_file_notthere_E();
            else
                throw annodir_file_unreadable_E();
        }
        std::auto_ptr<database_T > db(new database_T(*f));

        db->display(std::cout);
    }
    catch (annodir_file_notthere_E)
    {
        if (options.verbose())
            std::cout << "Warning: couldn't open " << keys["location"]
                << " for read(" << errno << "): " << strerror(errno)
                << std::endl;
    }
    catch (annodir_file_unreadable_E)
    {
        std::cout << "Error: couldn't open " << keys["location"]
            << " for read (" << errno << "): " << strerror(errno) << std::endl;
    }
}

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/action_list_handler.cc
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

#include "src/anno_db.hh"
#include "src/exceptions.hh"
#include "src/options.hh"
#include "src/action_list_handler.hh"

    int
action_list_handler_T::operator() (void)
{
    options_T options;
    try
    {
        /* load the db */
        std::auto_ptr<std::ifstream > f(new 
                std::ifstream(options.get_filename()));
        if (! (*f))
        {
            if (ENOENT == errno)
                throw annodir_file_notthere_E();
            else
                throw annodir_file_unreadable_E();
        }
        std::auto_ptr<anno_db_T > db(new anno_db_T(*f));

        /* display the db */
        db->display(std::cout);

    }
    catch (annodir_file_notthere_E)
    {
        /* file isn't there. not a problem, only whine if we're in verbose mode.
         * just don't display anything. */
        if (options.verbose())
            std::cout << "Warning: couldn't open " << options.get_filename()
                << " for read (" << errno << "): " << strerror(errno) 
                << std::endl;
    }
    catch (annodir_file_unreadable_E)
    {
        /* bleh! file there, can't read it. */
        std::cout << "Error: couldn't open " << options.get_filename()
            << " for read (" << errno << "): " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/action_export_handler.cc
 * $Id$
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

#include <src/action_export_handler.hh>
#include <src/database.hh>
#include <src/options.hh>
#include <src/exceptions.hh>

#include <cstdlib>
#include <errno.h>
#include <memory>
#include <fstream>

    int
action_export_handler_T::operator() (void)
{
    options_T options;

    try
    {
	std::auto_ptr<database_T > db(new database_T());

	{
	    /* load db */
	    std::auto_ptr<std::istream > in(new
		std::ifstream(options.get_filename().c_str()));

	    if ((*in))
		db->load(*in);
	    else
	    {
		if (ENOENT != errno)
		    throw annodir_file_unreadable_E();
	    }

	    /* export db */
	    std::auto_ptr<std::ostream > out(new
		std::ofstream(options.get_export().c_str()));

	    if ((*out))
		db->do_export(*out);
	    else
		throw annodir_file_unwriteable_E();
	}
    }
    catch (annodir_file_unreadable_E)
    {
        std::cout << "Error: couldn't open " << options.get_filename()
            << " for read (" << errno << "): " << strerror(errno)
            << std::endl;
        return EXIT_FAILURE;
    }
    catch (annodir_file_unwriteable_E)
    {
        std::cout << "Error: couldn't open " << options.get_export()
            << " for write (" << errno << "): " << strerror(errno)
            << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

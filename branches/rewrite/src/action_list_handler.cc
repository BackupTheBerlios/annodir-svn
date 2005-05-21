/*
 * annodir -- src/action_list_handler.cc
 * $Id$
 * Copyright (c) 2005 Ciaran McCreesh <ciaranm at gentoo.org>
 * Copyright (c) 2005 Aaron Walker <ka0ttic@gentoo.org>
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <memory>
#include "db.hh"
#include "exceptions.hh"
#include "action_list_handler.hh"

int
action_list_handler_T::operator() (const opts_type &opts)
{
    const util::string dbfile(optget("dbfile", util::string));
    
    try
    {
        std::auto_ptr<std::ifstream> f(new std::ifstream(dbfile.c_str()));
        if (not (*f))
            throw annodir_bad_file_E(dbfile);

        std::auto_ptr<db_T> db(new db_T(*f));
        db->display(std::cout);
    }
    catch (const annodir_bad_file_E &e)
    {
        if (errno == ENOENT and optget("verbose", bool))
            std::cerr << e.what() << std::endl;
        else if (errno != ENOENT)
        {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

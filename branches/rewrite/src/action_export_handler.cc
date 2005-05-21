/*
 * annodir -- src/action_export_handler.cc
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

#include "db.hh"
#include "action_export_handler.hh"

int
action_export_handler_T::operator() (const opts_type &opts)
{
    /* load database */
    std::auto_ptr<db_T> db(new db_T());
    {
        const util::string dbfile(optget("dbfile", util::string));
        std::auto_ptr<std::istream> f(new std::ifstream(dbfile.c_str()));
        if ((*f))
            db->load(*f);
        else if (errno != ENOENT)
            throw annodir_bad_file_E(dbfile);
    }

    /* export database */
    const util::string exfile(optget("exportfile", util::string));
    std::auto_ptr<std::ostream> f(new std::ofstream(exfile.c_str()));
    if (not (*f))
        throw annodir_bad_file_E(exfile);

    db->do_export(*f);

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/action_delete_handler.cc
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
#include "db_entry.hh"
#include "exceptions.hh"
#include "action_delete_handler.hh"

int
action_delete_handler_T::operator() (const opts_type &opts)
{
    util::string index;
    const util::string dbfile(optget("dbfile", util::string));

    /* load database */
    std::auto_ptr<db_T> db(new db_T());
    {
        std::auto_ptr<std::istream> f(new std::ifstream(dbfile.c_str()));
        if ((*f))
            db->load(*f);
        else if (errno != ENOENT)
            throw annodir_bad_file_E(dbfile);
    }

    if (opts.size() > 1)
        throw node_only_one_index_E();

    /* index not specified, prompt for one */
    if (opts.empty())
    {
        index.assign(util::get_user_input("Index"));
        if (index.empty())
            throw node_invalid_index_E("none");
    }
    else
        index.assign(opts.front());

    /* find index */
    db_T *node = db->find(index);
    if (not node)
        throw node_invalid_index_E(index);

    /* we have to call erase from the parent */
    db_T *parent = node->parent;

    db_T::iterator i;
    for (i = parent->begin() ; i != parent->end() ; ++i)
        if ((*i)->index() == node->index())
            break;

    parent->erase(i);
    node = NULL;

    /* save */
    {
        std::auto_ptr<std::ostream> f(new std::ofstream(dbfile.c_str()));
        if (not (*f))
            throw annodir_bad_file_E(dbfile);

        db->dump(*f);
    }

    if ((db->empty() and optget("delete on empty", bool)) and
        (unlink(dbfile.c_str()) != 0))
        throw annodir_bad_file_E(dbfile);

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

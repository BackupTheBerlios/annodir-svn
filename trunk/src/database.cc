/*
 * annodir -- src/database.cc
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

#include <cstdlib>

#include "src/database.hh"
#include "src/node_entry.hh"
#include "src/database_entry.hh"
#include "src/database_note_entry.hh"
#include "src/database_link_entry.hh"
#include "src/database_metadata_entry.hh"
#include "src/exceptions.hh"
#include "src/util.hh"

/*
 * Load top-level nodes (which in turn load their children)
 */
    void
database_T::load(std::istream &stream)
{
    while (not stream.eof())
    {
        /* get the header to determine what to create */
        std::string s;
        if (std::getline(stream, s))
        {
            util::debug_msg("db_T::load(): %s", s.c_str());

            /* strip trailing colon */
            if (s[s.length() - 1] != ':')
                throw item_not_parsable_E();
            s.erase(s.length() - 1);
            
            /* special case - metadata doesn't get its own node */
            if (database_metadata_entry_T::recognise_item(s))
            {
                entry = new database_metadata_entry_T(&stream, this);
                continue;
            }

            node_entry_T *node = new node_entry_T(this);

            /* try to find a relevant class */
            if (database_note_entry_T::recognise_item(s))
                node->entry = new database_note_entry_T(&stream, node);
            else if (database_link_entry_T::recognise_item(s))
                node->entry = new database_link_entry_T(&stream, node);
            else if (database_entry_T::recognise_item(s))
                node->entry = new database_entry_T(&stream, node);
            else
                throw item_not_recognised_E();

            push_back(node);
        }
    }
}

/* vim: set tw=80 sw=4 et : */

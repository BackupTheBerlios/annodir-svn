/*
 * annodir -- src/action_add_handler.cc
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

#include "config.h"
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <fstream>
#include <string>

#include "src/database.hh"
#include "src/node_entry.hh"
#include "src/exceptions.hh"
#include "src/options.hh"
#include "src/action_add_handler.hh"
#include "src/database_note_entry.hh"
#include "src/database_link_entry.hh"
#include "src/database_metadata_entry.hh"
#include "src/input.hh"

    database_entry_T *
make_new_entry(const char *type, node_entry_T *node)
{
    if (0 == strcasecmp(type, "prompt"))
    {
        char *in = get_user_input("Item Type");
        if (in)
        {
            return make_new_entry(in, node);
            free(in);
        }
        else
            return NULL;
    }

    else if (0 == strcasecmp(type, "note"))
        return new database_note_entry_T(node);
    else if (0 == strcasecmp(type, "link"))
        return new database_link_entry_T(node);
    else if (0 == strcasecmp(type, "metadata"))
        return new database_metadata_entry_T(node);
    else /* fallback */
        return new database_entry_T(node); 
}

/*
 * Deduce the node that corresponds to the given index string
 */

    node_entry_T *
get_node_with_index(node_entry_T *node, std::string const &index)
{
    node_entry_T *node_p = NULL;
    node_entry_T::iterator i;
    for (i = node->begin() ; i != node->end() ; i++)
    {
        node_p = *i;
        if ((node_p->index() == index) or 
            (node_p = get_node_with_index(node_p, index)))
            break;
    }
    return ((node_p ? (node_p->index() == index ? node_p : NULL) : NULL));
}

    int
action_add_handler_T::operator() (void)
{
    options_T options;

    try
    {
        bool exists = false;

        /* load database */
        database_T db;

        {
            std::auto_ptr<std::istream > f(new 
                    std::ifstream(options.get_filename().c_str()));
            if ((*f))
            {
                exists = true;
                db.load(*f);
            }
            else
            {
                if (ENOENT != errno)
                    throw annodir_file_unreadable_E();
            }
        }

        /* add metadata if the file didn't already exist */
        if (not exists)
        {
            db.entry = make_new_entry("metadata", &db);
            if (not db.entry)
                return EXIT_FAILURE;
            db.entry->set_new_object_defaults();
        }

        /* 
         * If an index was specified (--index), the new node will become a
         * child of that node.  Otherwise, the new node will become the last
         * top-level child (parent is the root node).
         */
        node_entry_T *parent = &db;
        if (not options.get_index().empty())
        {
            parent = get_node_with_index(&db, options.get_index());
            if (not parent)
                throw node_invalid_index_E();
        }

        /* add a new entry */
        node_entry_T *node = new node_entry_T(parent);
        node->entry = make_new_entry(options.get_type().c_str(), node);

        if (not node->entry)
            return EXIT_FAILURE;
        
        node->entry->set_new_object_defaults();

        if (node->entry->prompt_user_for_values())
        {
            parent->push_back(node);
            
            /* save */
            std::auto_ptr<std::ostream> f(new
                    std::ofstream(options.get_filename().c_str()));
            if ((*f))
                db.dump(*f);
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
    catch (node_invalid_index_E)
    {
        /* specified index is invalid */
        std::cout << "Error: invalid index '" << options.get_index()
            << "'." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

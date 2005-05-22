/*
 * annodir -- src/action_add_handler.cc
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
#include "db_entry.hh"
#include "db_note_entry.hh"
#include "db_link_entry.hh"
#include "db_meta_entry.hh"
#include "exceptions.hh"
#include "action_add_handler.hh"

static db_entry_T *
make_new_entry(const util::string &, db_T *, std::istream * = NULL);

static db_entry_T *
make_new_entry(const util::string &type, db_T *node, std::istream *stream)
{
    if (type == "prompt")
    {
        const util::string input(util::get_user_input("Item type"));
        if (input.empty())
            return NULL;

        return make_new_entry(input, node, stream);
    }
    else if (type == "note")
    {
        debug_msg("make_new_entry -> note");
        return new db_note_entry_T(stream, node);
    }
    else if (type == "link")
    {
        debug_msg("make_new_entry -> link");
        return new db_link_entry_T(stream, node);
    }
    else if (type == "metadata")
    {
        debug_msg("make_new_entry -> meta");
        return new db_meta_entry_T(stream, node);
    }
    else /* fallback */
    {
        debug_msg("make_new_entry -> fallback");
        return new db_entry_T(stream, node);
    }
}

int
action_add_handler_T::operator() (const opts_type &opts)
{
    bool exists = false;
    const util::string dbfile(optget("dbfile", util::string));

    /* load database */
    std::auto_ptr<db_T> db(new db_T());
    {
        std::auto_ptr<std::istream> f(new std::ifstream(dbfile.c_str()));
        if ((*f))
        {
            exists = true;
            db->load(*f);
        }
        else if (errno != ENOENT)
            throw annodir_bad_file_E(dbfile);
    }

    /* add metadata if the didn't already exist */
    if (not exists)
    {
        debug_msg("adding metadata");

        db_entry_T *meta = make_new_entry("metadata", db.get());
        if (not meta)
            return EXIT_FAILURE;

        db->entries.push_front(meta);
        db->entries.front()->set_new_object_defaults();
    }

    if (opts.size() > 1)
        throw node_only_one_index_E();

    /* 
     * If an index was specified, the new node will become a child of
     * that node.  Otherwise, the new node will become the last top-level
     * child (parent is the root node).
     */
    db_T *parent = NULL;

    if (opts.empty())
    {
        parent = db.get();
        debug_msg("Creating new top-level node");
    }
    else if (not (parent = db->find(opts.front())))
        throw node_invalid_index_E(opts.front());
    else
        debug_msg("Creating new sub-node (of index '%s')",
            opts.front().c_str());

    /* add new entry */
    db_T *node = new db_T(parent);
    {
        db_entry_T *entry =
            make_new_entry(optget("type", util::string), node);
   
        if (not entry)
            return EXIT_FAILURE;

        node->entries.push_back(entry);

        debug_msg("pusing back entry of type '%s'",
            optget("type", util::string).c_str());
    }

    node->entries.back()->set_new_object_defaults();

    if (node->entries.back()->prompt_user_for_values())
    {
        parent->push_back(node);

        /* save */
        std::auto_ptr<std::ostream> f(new std::ofstream(dbfile.c_str()));
        if (not (*f))
            throw annodir_bad_file_E(dbfile);

        db->dump(*f);
    }
    else
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */

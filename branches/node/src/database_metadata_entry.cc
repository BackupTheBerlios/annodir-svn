/*
 * annodir -- src/database_metadata_entry.cc
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

#include <errno.h>
#include <cstdlib>
#include <memory>
#include <fstream>
#include <unistd.h>

#include "src/database_metadata_entry.hh"
#include "src/node_entry.hh"
#include "src/exceptions.hh"
#include "src/database.hh"
#include "src/options.hh"
#include "src/util.hh"

database_metadata_entry_T::database_metadata_entry_T(node_entry_T *node)
    : database_entry_T(node)
{
    if (node)
        mynode = node;
}

/*
 * Create a new item read from the supplied stream.
 */
database_metadata_entry_T::database_metadata_entry_T(std::istream *stream,
    node_entry_T *node) : database_entry_T(stream, node)
{
    id = default_id();
}

/*
 * Get the default id for an object of our kind
 */
    std::string
database_metadata_entry_T::default_id()
{
    return "metadata";
}

/*
 * Do we recognise this item?
 */
    bool
database_metadata_entry_T::recognise_item(std::string item)
{
    return (item == "metadata");
}

    void
database_metadata_entry_T::set_new_object_defaults()
{
    char path[PATH_MAX + 1];
    database_entry_T::set_new_object_defaults();
    keys["title"].assign(util::basename(getcwd(path, PATH_MAX)));
}

/* 
 * metadata shouldn't be displayed
 */
    void
database_metadata_entry_T::display(std::ostream &stream)
{
    return;
}

/* vim: set tw=80 sw=4 et : */

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

database_metadata_entry_T::database_metadata_entry_T(const node_entry_T *node)
    : database_entry_T(node)
{
    id = default_id();
    util::debug_msg("Initializing new entry of type '%s'", id.c_str());
}

/*
 * Create a new item read from the supplied stream.
 */
database_metadata_entry_T::database_metadata_entry_T(std::istream *stream,
    const node_entry_T *node) : database_entry_T(stream, node)
{
    id = default_id();
    util::debug_msg("Initializing new entry of type '%s'", id.c_str());
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
 * Dump our data to the supplied stream.
 * NOTE: this is identical to database_entry_T::dump() with the 
 * exception that "end" needs to be printed BEFORE child entries.
 */
    void
database_metadata_entry_T::dump(std::ostream &stream)
{
    /* block header */
    stream << mynode->indent() << id << ":" << std::endl;

    /* entries */
    std::map<std::string, std::string >::iterator i;
    for (i = keys.begin() ; i != keys.end() ; ++i)
        stream << mynode->indent() << "  " << i->first
            << "=" << i->second << std::endl;

    /* end */
    stream << mynode->indent() << "end" << std::endl;
    
    /* recurse through child nodes */
    mynode->recurse(&database_entry_T::dump, stream);
}

    void
database_metadata_entry_T::do_export(std::ostream &stream)
{
    stream << "[" << keys["title"] << "] " << std::endl;
    database_entry_T::do_export(stream);
}

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/node_entry.hh
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

#ifndef NODE_ENTRY_HH
#define NODE_ENTRY_HH 1

#include <string>
#include <vector>
#include "src/database_entry.hh"

class node_entry_T
{
    public:
        friend class database_entry_T;

	node_entry_T(node_entry_T *parent_node = NULL);
	~node_entry_T();
        
	std::vector<node_entry_T * > children;
        database_entry_T *entry;

        std::string const &index();
        std::string const &indent() { return node_entry_T::indent_str; }

    private:
        std::vector<int > _index;
        std::string index_str;
        std::string indent_str;

        node_entry_T *parent;   /* parent */
        node_entry_T *prev;     /* previous sibling */
        node_entry_T *next;     /* next sibling */
};

#endif

/* vim: set tw=80 sw=4 et : */

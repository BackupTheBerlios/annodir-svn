/*
 * annodir -- src/node_entry.cc
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

#include <string>
#include <cstdio>
#include "src/node_entry.hh"
#include "src/util.hh"

/*
 * Initialize a node instance
 * Set parent, prev, and next pointers to their respective nodes
 */
node_entry_T::node_entry_T(node_entry_T *parent_node)
{
    parent = parent_node;
    prev = next = 0;

    /* child with siblings */
    if (parent and !(parent->children.empty()))
    {
        prev = parent->children.back();
        prev->next = this;
        _index = prev->_index;
        _index.back()++;
        indent_str = prev->indent_str;
    }
    /* only child */
    else if (parent)
    {
        _index = parent->_index;
        _index.push_back(1);
        indent_str = parent->indent_str + "  ";
    }
    /* root */
    else
    {
        _index.push_back(0);
        return;
    }

    /* perhaps an index class is needed... */

    /* construct index string */
    std::vector<int >::iterator i;
    for (i = _index.begin() + 1 ; i != _index.end() ; ++i)
    {
        /* is there a C++ way of doing this? */
        char buf[8];
        snprintf(buf, sizeof(buf), "%d.", *i);
        index_str.append(buf);
    }

    /* chop trailing '.' */
    if (index_str[index_str.length() - 1] == '.')
        index_str.erase(index_str.length() - 1);
    
    util::debug_msg("initialized new node with index %s", index_str.c_str());
}

/* Tidy up */
node_entry_T::~node_entry_T()
{
    delete entry;
}

/* vim: set tw=80 sw=4 et : */

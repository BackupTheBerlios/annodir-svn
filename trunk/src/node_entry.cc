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

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "src/node_entry.hh"
#include "src/database_entry.hh"
#include "src/util.hh"

/*
 * Initialize a node instance.
 *  - Set parent and sibling pointers to their respective nodes
 *  - Construct the index and it's string representation
 *  - Construct indentation string of the appropriate width
 */
node_entry_T::node_entry_T(const node_entry_T *parent_node)
    : _prev(0), _next(0)
{
    _parent = const_cast<node_entry_T * > (parent_node);

    /* child with siblings */
    if (_parent and not _parent->empty())
    {
        _prev = _parent->back();
        _prev->_next = this;

        _index = _prev->_index;
        _index.back()++;

        indent_str = _prev->indent_str;
    }
    /* only child */
    else if (_parent)
    {
        _index = _parent->_index;
        _index.push_back(1);

        indent_str = _parent->indent_str;
        /* top-level children nodes shouldn't be indented */
        if (_parent->_parent)
            indent_str.append("  ");
    }
    /* root */
    else
    {
        _index.push_back(0);
        return;
    }

    /* initial index string */
    index_str = index();
    util::debug_msg("initialized new node with index %s", index_str.c_str());
}

/*
 * Construct index string representation 
 */
    std::string const&
node_entry_T::index()
{
    index_str.clear();

    /* skip first element, as we care less about the root node (index 0) */
    std::vector<int >::iterator i;
    for (i = _index.begin() + 1 ; i != _index.end() ; ++i)
        index_str.append(util::sprintf("%d.", *i));

    /* chop trailing '.' */
    if (index_str[index_str.length() - 1] == '.')
        index_str.erase(index_str.length() - 1);

    return index_str;
}

/*
 * Recurse through child nodes running the specified function for each
 */
    void
node_entry_T::recurse(void (database_entry_T::*fp)(std::ostream&),
    std::ostream &stream)
{
    for (node_entry_T::iterator i = begin() ; i != end() ; ++i)
        ((*i)->entry->*fp)(stream);
}

/*
 * Tidy up
 */
node_entry_T::~node_entry_T()
{
    for (node_entry_T::iterator i = begin() ; i != end() ; ++i)
        delete *i;
}

/* vim: set tw=80 sw=4 et : */
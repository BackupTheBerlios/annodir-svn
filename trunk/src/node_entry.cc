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
#include <algorithm>
#include <functional>

#include "src/node_entry.hh"
#include "src/database_entry.hh"
#include "src/util.hh"

/*
 * Initialize a node instance.
 *  - Set parent and sibling pointers to their respective nodes
 *  - Construct the index and it's string representation
 *  - Construct indentation string of the appropriate width
 */
node_entry_T::node_entry_T(node_entry_T *parent_node)
    : _prev(NULL), _next(NULL)
{
    _parent = parent_node;

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
 * Deduce the node that corresponds to the given index string
 */

    node_entry_T *
node_entry_T::find_index(std::string const &index)
{
    node_entry_T *node_p = NULL;
    for (node_entry_T::iterator i = begin() ; i != end() ; i++)
    {
        node_p = *i;
        if ((node_p->index() == index) or (node_p = node_p->find_index(index)))
            break;
    }
    return ((node_p ? (node_p->index() == index ? node_p : NULL) : NULL));
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
        ((*i)->entry()->*fp)(stream);
}

/*
 * Insert entry ; if front is true insert at front (defaults to false)
 */
    void
node_entry_T::insert_entry(database_entry_T *e, bool front /* = false */)
{
    if (front)
        _entries.insert(_entries.begin(), e);
    else
        _entries.push_back(e);
}

/*
 * Move the entry with the specified id to the front and return it
 */
    bool
entry_is(database_entry_T *e, std::string id) { return e->is(id); }

    database_entry_T *
node_entry_T::entry(std::string id /* = "" */)
{
    if (_entries.empty())
        return NULL;

    /* default arg ("") means return the a pointer to the first entry */
    if (id.empty())
        return _entries.front();

    std::stable_partition(_entries.begin(), _entries.end(),
            std::bind2nd(ptr_fun(entry_is), id));
    return _entries.front();
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

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
#include <cstdlib>
#include "src/node_entry.hh"
#include "src/util.hh"

node_entry_T::node_entry_T()
{

}

/*
 * Initialize a node instance.
 *  - Set parent and sibling pointers to their respective nodes
 *  - Construct the index and it's string representation
 *  - Construct indentation string of the appropriate width
 */
node_entry_T::node_entry_T(node_entry_T *parent_node)
{
    parent = parent_node;
    prev = next = 0;

    /* child with siblings */
    if (parent and not (parent->children.empty()))
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

        indent_str = parent->indent_str;
        /* top-level children nodes shouldn't be indented */
        if (parent->parent)
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
    {
#ifdef HAVE_ASPRINTF
        char *buf;
        std::asprintf(&buf, "%d.", *i);
#else
        char buf[10] = { 0 };
        std::snprintf(buf, sizeof(buf), "%d.", *i);
#endif
        index_str.append(buf);
#ifdef HAVE_ASPRINTF
        std::free(buf);
#endif
    }

    /* chop trailing '.' */
    if (index_str[index_str.length() - 1] == '.')
        index_str.erase(index_str.length() - 1);

    return index_str;
}


/*
 * Tidy up
 */
node_entry_T::~node_entry_T()
{
    std::vector<node_entry_T * >::iterator i;
    for (i = children.begin() ; i != children.end() ; ++i)
        delete *i;
}

/* vim: set tw=80 sw=4 et : */

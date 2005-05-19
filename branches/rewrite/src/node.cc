/*
 * annodir -- src/node.cc
 * $Id$
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

#include "node.hh"

/*
 * Initialize a new node instance.
 *  - Set parent and sibling pointers to their respective nodes.
 *  - Construct initial index string.
 *  - Construct initial indent string.
 */

node_T::node_T(node_T *parent) : _prev(NULL), _next(NULL)
{
    this->_parent = parent;

    /* child with siblings */
    if (this->_parent and not this->_parent->empty())
    {
        this->_prev = this->_parent->back();
        this->_prev->_next = this;

        this->_index = this->_prev->_index;
        this->_index.back()++;

        this->_indent_str = this->_prev->_indent_str;
    }
    /* only child */
    else if (this->_parent)
    {
        this->_index = this->_parent->_index;
        this->_index.push_back(1);

        this->_indent_str = this->_parent->_indent_str;

        /* top-level children nodes should not be indented */
        if (this->_parent->_parent)
            this->_indent_str.append("  ");
    }
    /* root node */
    else
    {
        this->_index.push_back(0);
        return;
    }

    /* initial index string */
    this->_index_str = this->index();
}

/*
 * Deduce the node that corresponds to the specified index.
 * Returns NULL if not found.
 * FIXME: should we behave like other std containers wrt to
 * find (ie. return an iterator or end() if not found)?
 */

node_T *
node_T::find(const util::string &index)
{
    node_T *node_p = NULL;

    for (iterator i = this->begin() ; i != this->end() ; ++i)
    {
        node_p = *i;

        if ((node_p->index() == index) or (node_p = node_p->find(index)))
            break;
    }

    return ((node_p ? (node_p->index() == index ? node_p : NULL) : NULL));
}

/*
 * Construct index string
 */

const util::string &
node_T::index()
{
    this->_index_str.clear();

    /* skip first element, as we care less about the root node (index 0) */
    std::vector<unsigned short>::iterator i;
    for (i = this->_index.begin() + 1 ; i != this->_index.end() ; ++i)
        this->_index_str.append(util::sprintf("%d.", *i));

    /* chop trailing '.' */
    this->_index_str.erase(this->_index_str.length() - 1);

    return this->_index_str;
}

/*
 * Tidy up our child nodes
 */

node_T::~node_T()
{
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        delete *i;
}

/* vim: set tw=80 sw=4 et : */

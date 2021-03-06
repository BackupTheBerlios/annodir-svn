/*
 * annodir -- src/db.cc
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

#include <functional>
#include <algorithm>

#include "db_entry.hh"
#include "db_note_entry.hh"
#include "db_link_entry.hh"
#include "db_meta_entry.hh"
#include "db.hh"

void
db_T::init(db_T *parent)
{
    this->parent = parent;

    /* child with siblings */
    if (this->parent and not this->parent->empty())
    {
        this->prev = this->parent->back();
        this->prev->next = this;

        this->_indexv = this->prev->_indexv;
        this->_indexv.back()++;

        this->_indent = this->prev->_indent;
    }
    /* only child */
    else if (this->parent)
    {
        this->_indexv = this->parent->_indexv;
        this->_indexv.push_back(1);

        this->_indent = this->parent->_indent;

        /* only indent non-top-level nodes */
        if (this->parent->parent)
            this->_indent += "  ";
    }
    /* root node */
    else
    {
        this->_indexv.push_back(0);
        return;
    }

    /* construct initial index string */
    this->_index = this->index();
}

/*
 * Load top-level nodes (which in turn load their children).
 */

void
db_T::load(std::istream &stream)
{
    while (not stream.eof())
    {
        /* get header and determine entry type */
        std::string s;
        if (std::getline(stream, s))
        {
            util::string line(s);

            /* strip leading whitespace */
            util::string::size_type pos = line.find_first_not_of(" \t");
            if (pos != util::string::npos)
                line.erase(0, pos);

            /* strip trailing colon */
            if (line[line.length() - 1] != ':')
                throw item_not_parsable_E();

            line.erase(line.length() - 1);

            /* special case - metadata does not get it's own node */
            if (db_meta_entry_T::recognise_item(line))
            {
                this->entries.push_front(new db_meta_entry_T(&stream, this));
                continue;
            }

            db_T *node = new db_T(this);

            /* try to find relevant entry type */
            if (db_note_entry_T::recognise_item(line))
                node->entries.push_back(new db_note_entry_T(&stream, node));
            else if (db_link_entry_T::recognise_item(line))
                node->entries.push_back(new db_link_entry_T(&stream, node));
            else if (db_entry_T::recognise_item(line))
                node->entries.push_back(new db_entry_T(&stream, node));
            else
            {
                delete node;
                throw item_not_recognised_E();
            }

            this->push_back(node);
        }
    }
}

/*
 * Dump the database to the specified stream.
 */

void
db_T::dump(std::ostream &stream)
{
    /* dump our entries */
    entries_type::iterator e;
    for (e = this->entries.begin() ; e != this->entries.end() ; ++e)
    {
        (*e)->dump(stream);
        
        if (not this->parent or (*e)->is("metadata") or
            (this->parent and this->empty()))
            stream << this->_indent << "end" << std::endl;
    }

    /* dump our child nodes */
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->dump(stream);

    if (this->parent and not this->empty() and not this->is_link())
        stream << this->_indent << "end" << std::endl;
}

/*
 * Display the database (pretty-like) on the specified stream.
 */

void
db_T::display(std::ostream &stream)
{
    /* display our entries */
    entries_type::iterator e;
    for (e = this->entries.begin() ; e != this->entries.end() ; ++e)
        (*e)->display(stream);

    /* display our child nodes */
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->display(stream);
}

/*
 * Export the database to the specified stream.
 */

void
db_T::do_export(std::ostream &stream)
{
    /* export our entries */
    entries_type::iterator e;
    for (e = this->entries.begin() ; e != this->entries.end() ; ++e)
        (*e)->do_export(stream);

    /* export our child nodes */
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->do_export(stream);
}

/*
 * Deduce the node that corresponds to the specified index.
 * Returns NULL if not found.
 * FIXME: should we behave like other std containers wrt to
 * find (ie. return an iterator or end() if not found)?
 */

db_T *
db_T::find(const util::string &index)
{
    db_T *node_p = NULL;

    for (iterator i = this->begin() ; i != this->end() ; ++i)
    {
        node_p = *i;
        if ((node_p->index() == index) or (node_p = node_p->find(index)))
            break;
    }

    return ((node_p ? (node_p->index() == index ? node_p : NULL) : NULL));
}

/*
 * Construct an index string from our internal index vector
 * and return it.
 */

const util::string &
db_T::index()
{
    this->_index.clear();

    /* skip first element, as we care less about the root node (index 0) */
    std::vector<unsigned short>::iterator i;
    for (i = this->_indexv.begin() + 1 ; i != this->_indexv.end() ; ++i)
        this->_index += util::sprintf("%d.", *i);

    /* chop trailing '.' */
    if (this->_index.length() > 1)
        this->_index.erase(this->_index.length() - 1);

    return this->_index;
}

/*
 * Pretend to be our parent node.  This does not modify node data
 * whatsoever, just the index/indent.  You could say the node is
 * "pretending" to be it's parent.  This is used in certain special
 * cases.  For example, if a node is a link to another db_T, it will
 * have a metadata entry (which gets it's own node).  Without
 * pretending to be the parent, the first visible child would get
 * a sub-index of the metadata node, which display-wise is incorrect.
 */

void
db_T::become_parent()
{
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->become_parent();

    if (this->size() > 1)
    {
        for (iterator i = this->begin() + 1 ; i != this->end() ; ++i)
        {
            (*i)->_indexv = (*i)->prev->_indexv;
            ((*i)->_indexv.back())++;
        }
    }

    if (this->parent)
    {
        this->_indexv = this->parent->_indexv;
        this->_index  = this->parent->index();
        this->_indent = this->parent->_indent;
    }
}

/*
 * Tidy up.
 */

db_T::~db_T()
{
    /* entries */
    entries_type::iterator i;
    for (i = this->entries.begin() ; i != this->entries.end() ; ++i)
        delete *i;

    /* child nodes */
    for (iterator n = this->begin() ; n != this->end() ; ++n)
        delete *n;
}

/* vim: set tw=80 sw=4 et : */

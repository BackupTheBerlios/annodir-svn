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

#include <iostream>
#include <string>
#include <vector>
#include "src/database_entry.hh"

class node_entry_T : public std::vector<node_entry_T * >
{
    private:
        std::vector<int > _index;
        std::string index_str;
        std::string indent_str;

    protected:
        node_entry_T *_parent;   /* parent */
        node_entry_T *_prev;     /* previous sibling */
        node_entry_T *_next;     /* next sibling */

    public:
	node_entry_T(const node_entry_T *parent_node = NULL);
	virtual ~node_entry_T();
        
        database_entry_T *entry;

        /* public interfaces to private members */
        node_entry_T *parent() const { return _parent; }
        node_entry_T *next() const { return _next; }
        node_entry_T *prev() const { return _prev; }
        std::string const &indent() const { return indent_str; }

        /* actions */
        std::string const &index();
        node_entry_T *find_index(std::string const &index);

        virtual void recurse(void (database_entry_T::*fp)(std::ostream&),
            std::ostream &stream);
        virtual void dump(std::ostream &stream) { entry->dump(stream); }
        virtual void display(std::ostream &stream) { entry->display(stream); }
        virtual void do_export(std::ostream &stream) { entry->do_export(stream); }
};

#endif

/* vim: set tw=80 sw=4 et : */

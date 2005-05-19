/*
 * annodir -- src/node.hh
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

#ifndef HAVE_NODE_HH
#define HAVE_NODE_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "common.hh"

class node_T : public std::vector<node_T * >
{
    public:
        node_T(node_T *parent = NULL);
        virtual ~node_T();

        node_T * &parent() const { return this->_parent; }
        node_T * &next() const { return this->_next; }
        node_T * &prev() const { return this->_prev; }

        const util::string &indent() const { return this->_indent_str; }
        const util::string &index();

        node_T *find(const util::string &);

        /* actions */
        virtual void do_dump(std::ostream &)    = 0;
        virtual void do_load(std::istream &)    = 0;
        virtual void do_display(std::ostream &) = 0;
        virtual void do_export(std::ostream &)  = 0;

    protected:
        class entry_keys_T : public std::map<util::string, util::string>
        {
            public:
                const util::string &
                get_with_default(const util::string &, const util::string &);
        };

        entry_keys_T _keys;
        std::vector<unsigned short> _index;
        node_T *_parent, *_prev, *_next;
        util::string _index_str, _indent_str;
};

#endif

/* vim: set tw=80 sw=4 et : */

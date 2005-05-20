/*
 * annodir -- src/db.hh
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

#ifndef HAVE_DB_HH
#define HAVE_DB_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "common.hh"
#include "db_entry.hh"
#include "node.hh"

class db_T : public node_T
{
    public:
        typedef db_entry_T entry_type;
        typedef std::deque<entry_type * > entries_type;

        db_T(db_T *parent = NULL) : node_T(parent) { }
        db_T(std::istream *stream, db_T *parent = NULL)
            : node_T(parent) { load(*stream); }
        virtual ~db_T();

        virtual void load(std::istream &);
        virtual void dump(std::ostream &);
        virtual void display(std::ostream &);
        virtual void do_export(std::ostream &);

        entry_type *entry(const util::string &id = "");
        entries_type entries;
};

#endif

/* vim: set tw=80 sw=4 et : */

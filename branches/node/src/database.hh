/*
 * annodir -- src/database.hh
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

#ifndef HAVE_DATABASE_HH
#define HAVE_DATABASE_HH 1

#include <iostream>
#include "src/node_entry.hh"

class database_T : public node_entry_T
{
    public:
        database_T(node_entry_T *parent_node = NULL);
        database_T(std::istream &stream, node_entry_T *parent_node = NULL);
        virtual ~database_T();

        virtual void load(std::istream &stream);
        virtual bool dump(std::ostream &stream) { return entry->dump(stream); }
        virtual void display(std::ostream &stream) { entry->display(stream); }
        virtual void do_export(std::ostream &stream) { entry->do_export(stream); }
};

#endif

/* vim: set tw=80 sw=4 et : */

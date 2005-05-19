/*
 * annodir -- src/db.hh
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

#ifndef HAVE_DB_HH
#define HAVE_DB_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "common.hh"
#include "node.hh"

class db_T : public node_T
{
    public:
        db_T(db_T *parent = NULL);
        db_T(db_T *parent = NULL, std::istream *stream = NULL);
        virtual ~db_T() { }

        virtual void do_dump(std::ostream &);
        virtual void do_load(std::istream &);
        virtual void do_display(std::ostream &);
        virtual void do_export(std::ostream &);

        virtual void set_new_object_defaults();
        virtual void prompt_user_for_values();
};

#endif

/* vim: set tw=80 sw=4 et : */

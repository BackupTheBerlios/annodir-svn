/*
 * annodir -- src/database_metadata_entry.hh
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

#ifndef HAVE_DATABASE_METADATA_ENTRY_HH
#define HAVE_DATABASE_METADATA_ENTRY_HH 1

#include "src/database.hh"
#include "src/database_entry.hh"
#include "src/node_entry.hh"

class database_metadata_entry_T : public database_entry_T
{
    protected:
	virtual std::string default_id();

    public:
        database_metadata_entry_T(node_entry_T *node = NULL);
	database_metadata_entry_T(std::istream *stream = NULL,
            node_entry_T *node = NULL);

	static bool recognise_item(std::string item);
	virtual void set_new_object_defaults();

        virtual bool dump(std::ostream &stream);
};

#endif

/* vim: set tw=80 sw=4 et : */

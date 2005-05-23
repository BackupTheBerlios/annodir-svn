/*
 * annodir -- src/db_link_entry.hh
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

#ifndef HAVE_DB_LINK_ENTRY_HH
#define HAVE_DB_LINK_ENTRY_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <memory>
#include "db_entry.hh"

class db_link_entry_T : public db_entry_T
{
    public:
        db_link_entry_T(db_T *node = NULL)
            : db_entry_T(node), _linkdb(new db_T(node))
        { this->_id = this->default_id(); }
        db_link_entry_T(std::istream *stream, db_T *node = NULL);
        virtual ~db_link_entry_T() { }//if (this->_linkdb) delete this->_linkdb; }

        virtual void load(std::istream &);
        virtual void dump(std::ostream &);
        virtual void display(std::ostream &);
        virtual void set_new_object_defaults();
        virtual bool prompt_user_for_values();

        static bool recognise_item(const util::string &id)
        { return (id == "link"); }

    protected:
        virtual const util::string default_id() const { return "link"; }

    private:
        const std::auto_ptr<db_T> _linkdb;
};

#endif

/* vim: set tw=80 sw=4 et : */

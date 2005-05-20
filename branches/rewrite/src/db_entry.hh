/*
 * annodir -- src/db_entry.hh
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

#ifndef HAVE_DB_ENTRY_HH
#define HAVE_DB_ENTRY_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "common.hh"
#include "db_entry.hh"

class db_T;

class db_entry_T
{
    public:
        db_entry_T(db_T *node = NULL)
            : _id(default_id()), _padding("   "), _mynode(node) { }
        db_entry_T(std::istream *stream = NULL, db_T *node = NULL);
        virtual ~db_entry_T() { }

        virtual void load(std::istream &stream);
        virtual void dump(std::ostream &stream);
        virtual void display(std::ostream &stream);
        virtual void do_export(std::ostream &stream);

        virtual void set_new_object_defaults();
        virtual bool prompt_user_for_values() { return true; }

        static bool recognise_item(const util::string &) { return true; }
        bool is(const util::string &item) const { return (item == this->_id); }

        class entry_keys_T : public std::map<util::string, util::string>
        {
            public:
                const util::string &get_with_default
                    (const util::string &, const util::string &);
        };

        entry_keys_T keys;

    protected:
        virtual const util::string default_id() const { return "item"; }
        const util::string _id;
        const util::string _padding;

        /* pointer to node container the entry instance belongs to */
        db_T *_mynode;
};

#endif

/* vim: set tw=80 sw=4 et : */

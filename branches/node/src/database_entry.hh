/*
 * annodir -- src/database_entry.hh
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
#ifndef HAVE_DATABASE_ENTRY_HH
#define HAVE_DATABASE_ENTRY_HH 1

#include <map>
#include <string>
#include <iostream>

class node_entry_T;

class database_entry_keys_T : public std::map<std::string, std::string >
{
    public:
        std::string& get_with_default(std::string key,
                std::string default_value);
};

class database_entry_T
{
    public:
        database_entry_T(node_entry_T *node = NULL);
        database_entry_T(std::istream *stream = NULL, 
            node_entry_T *node = NULL);
        virtual ~database_entry_T() { };

        virtual bool dump(std::ostream &stream);
        virtual void load(std::istream &stream);
        virtual void display(std::ostream &stream);
        virtual void do_export(std::ostream &stream);

        database_entry_keys_T keys;

        static bool recognise_item(std::string item);

        virtual void set_new_object_defaults();
        virtual bool prompt_user_for_values();

    protected:
        virtual std::string default_id();
        std::string id;
        node_entry_T *mynode;
};

#endif

/* vim: set tw=80 sw=4 et : */

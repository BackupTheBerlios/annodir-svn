/*
 * annodir -- src/database_entry.hh
 * Copyright (c) 2004 Ciaran McCreesh <ciaranm at gentoo.org>
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
#ifndef HAVE_ANNO_DB_ENTRY_HH
#define HAVE_ANNO_DB_ENTRY_HH 1

#include "config.h"
#include <map>
#include <string>
#include <iostream>

class anno_db_entry_keys_T : public std::map<std::string, std::string >
{
    public:
        std::string& get_with_default(std::string key,
                std::string default_value);
};

class anno_db_entry_T
{
    public:
        anno_db_entry_T(std::istream *stream = NULL);

        virtual bool dump(std::ostream &stream);
        virtual void load(std::istream &stream);
        virtual void display(std::ostream &stream);

        anno_db_entry_keys_T keys;

        static bool recognise_item(std::string item);

        virtual void set_new_object_defaults();
        virtual void prompt_user_for_values();

    protected:
        virtual std::string default_id();
        std::string id;
};

#endif

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/db_link_entry.cc
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "db.hh"
#include "exceptions.hh"
#include "db_link_entry.hh"

void
db_link_entry_T::set_new_object_defaults()
{
    db_entry_T::set_new_object_defaults();
    this->keys["title"] = "Link";
}

bool
db_link_entry_T::prompt_user_for_values()
{
    if (not db_entry_T::prompt_user_for_values())
        return false;

    util::string input(util::get_user_input("Location",
        this->keys["location"]));

    if (input.empty())
        return false;

    try
    {
        if (not util::is_file(input))
        {
            /* hack in case a ' ' gets appended due to completion */
            input.erase(input.size() - 1);
            if (not util::is_file(input))
                throw annodir_bad_file_E(input);
        }

        this->keys["location"] = input;
    }
    catch (const annodir_bad_file_E &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    util::string def_title(util::dirname(this->keys["location"]));
    util::string title(util::get_user_input("Title", def_title));
    if (title.empty())
        return false;

    this->keys["title"] = title;
    return true;
}

void
db_link_entry_T::load(std::istream &stream)
{
    std::string s;
    while (std::getline(stream, s) and s != "end")
    {
        util::string line(s);

        /* strip leading whitespace */
        util::string::size_type pos = line.find_first_not_of(" \t");
        if (pos != util::string::npos)
            line.erase(0, pos);

        /* split on '=' */
        if ((pos = line.find('=')) != util::string::npos)
            this->keys[line.substr(0, pos)] = line.substr(pos + 1);
        else
            this->keys[line] = "Undefined";
    }

    /* load database */
    std::auto_ptr<std::ifstream>
        f(new std::ifstream(this->keys["location"].c_str()));
    if (not (*f))
        throw annodir_bad_file_E(this->keys["location"]);

    this->_mynode->load(*f);
}

void
db_link_entry_T::dump(std::ostream &stream)
{
    stream << this->_mynode->parent->indent() << this->_id << ":"
        << std::endl;

    entry_keys_T::iterator i;
    for (i = this->keys.begin() ; i != this->keys.end() ; ++i)
        stream << this->_mynode->parent->indent() << "  "
            << i->first << "=" << i->second << std::endl;

    stream << this->_mynode->parent->indent() << "end" << std::endl;

    try
    {
        /* dump linked db */
        std::auto_ptr<std::ostream>
            f(new std::ofstream(this->keys["location"].c_str()));
        if (not (*f))
            throw annodir_bad_file_E(this->keys["location"]);

        this->_mynode->dump(*f);
    }
    catch (const annodir_bad_file_E &e)
    {
        if (errno == ENOENT and optget("verbose", bool))
            std::cerr << e.what() << std::endl;
        else if (errno != ENOENT)
            std::cerr << e.what() << std::endl;
    }
}

void
db_link_entry_T::display(std::ostream &stream)
{
    stream << this->_mynode->indent() << this->_mynode->index()
        << ". " << this->keys["title"];

    if (not optget("summarise", bool))
    {
        if (optget("verbose", bool))
        {
            util::string date("(no date)");
            {
                entry_keys_T::iterator pos = this->keys.find("created_at");
                if (pos != this->keys.end())
                    date.assign(util::format_date(pos->second));
            }

            if (optget("compact", bool))
                stream << " ["
                    << this->keys.get_with_default("created_by","(anonymous)")
                    << ", " << date << "]";
            else
                stream << std::endl << this->_mynode->indent()
                    << this->_mynode->indent() << this->_padding
                    << "Created by "
                    << this->keys.get_with_default("created_by","(anonymous)")
                    << ", " << date;
        }

        stream << std::endl;

        this->_mynode->display(stream);
    }
    else
        stream << std::endl;
}

/* vim: set tw=80 sw=4 et : */

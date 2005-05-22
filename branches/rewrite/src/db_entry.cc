/*
 * annodir -- src/db_entry.cc
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
#include "db_note_entry.hh"
#include "db_link_entry.hh"
#include "db_meta_entry.hh"
#include "db_entry.hh"

const util::string &
db_entry_T::entry_keys_T::get_with_default(const util::string &key,
                                           const util::string &default_value)
{
    iterator pos = this->find(key);
    if (pos != this->end())
        return pos->second;
    else
        return default_value;
}

db_entry_T::db_entry_T(std::istream *stream, db_T *node)
    : _padding("   "), _mynode(node)
{
    this->_id = this->default_id();

    if (stream)
        this->load(*stream);
    else
        this->set_new_object_defaults();
}

/*
 * Create default settings
 */

void
db_entry_T::set_new_object_defaults()
{
    this->keys["created_by"] = optget("user", util::string);
    this->keys["created_at"] = util::sprintf("%lu",
            static_cast<unsigned long>(time(NULL)));
}

/*
 * Load database entry
 */

void
db_entry_T::load(std::istream &stream)
{
    std::string s;
    while (std::getline(stream, s))
    {
        util::string line(s);

        /* strip leading whitespace */
        util::string::size_type pos = line.find_first_not_of(" \t");
        if (pos != util::string::npos)
            line.erase(0, pos);

        /* sub entry? */
        if (line[line.length() - 1] == ':')
        {
            line.erase(line.length() - 1);

            /* new child node */
            db_T *node = new db_T(this->_mynode);

            /* create relevant entry type */
            if (db_note_entry_T::recognise_item(line))
                node->entries.push_back(new db_note_entry_T(&stream, node));
            else if (this->recognise_item(line))
                node->entries.push_back(new db_entry_T(&stream, node));
            else
                throw item_not_recognised_E();

            this->_mynode->push_back(node);
        }

        if (line == "end")
            break;

        /* split on '=' */
        if ((pos = line.find('=')) != util::string::npos)
            this->keys[line.substr(0, pos)] = line.substr(pos + 1);
        else
            this->keys[line] = "undefined";
    }
}

void
db_entry_T::dump(std::ostream &stream)
{
    /* block header */
    stream << this->_mynode->indent() << this->_id << ":" << std::endl;

    /* keys/values */
    entry_keys_T::iterator i;
    for (i = this->keys.begin() ; i != this->keys.end() ; ++i)
        stream << this->_mynode->indent() << "  " << i->first
            << "=" << i->second << std::endl;
}

void
db_entry_T::display(std::ostream &stream)
{
    stream << this->_mynode->indent() << this->_id << ":" << std::endl;

    entry_keys_T::iterator i;
    for (i = this->keys.begin() ; i != this->keys.end() ; ++i)
        stream << this->_mynode->indent() << i->first << "=" << i->second
            << std::endl;
}

void
db_entry_T::do_export(std::ostream &stream)
{
    if (this->_id != "metadata")
        stream << this->_mynode->indent() << "[" << this->_id << "] "
            << this->keys.get_with_default("title", "Untitled")
            << std::endl;

    if (not this->keys["body"].empty())
        stream << this->_mynode->indent()
            << this->keys.get_with_default("body", "(no text)")
            << std::endl;

    stream << this->_mynode->indent() << "created by: "
        << this->keys.get_with_default("created_by", "(anonymous)");

    util::string date("(no date)");
    {
        entry_keys_T::iterator pos = this->keys.find("created_at");
        if (pos != this->keys.end())
            date.assign(util::format_date(pos->second));
    }

    stream << " on: " << date;

    if(not this->keys["priority"].empty())
        stream << " with priority: "
            << this->keys.get_with_default("priority", "medium");

    stream << std::endl << std::endl;
}

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/db.cc
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

#include <functional>
#include <algorithm>

#include "db_entry.hh"
#include "db_note_entry.hh"
#include "db_link_entry.hh"
#include "db_meta_entry.hh"
#include "db.hh"

/*
 * Load top-level nodes (which in turn load their children).
 */

void
db_T::load(std::istream &stream)
{
    while (not stream.eof())
    {
        /* get header and determine entry type */
        std::string s;
        if (std::getline(stream, s))
        {
            util::string line(s);

            /* strip leading whitespace */
            util::string::size_type pos = line.find_first_not_of(" \t");
            if (pos != util::string::npos)
                line.erase(0, pos);

            /* strip trailing colon */
            if (line[line.length() - 1] != ':')
                throw item_not_parsable_E();

            line.erase(line.length() - 1);

            /* special case - metadata does not get it's own node */
            if (db_meta_entry_T::recognise_item(line))
            {
                this->entries.push_front
                    (new db_meta_entry_T(&stream, this));
                continue;
            }

            db_T *node = new db_T(this);

            /* try to find relevant entry type */
            if (db_note_entry_T::recognise_item(line))
                node->entries.push_back(new db_note_entry_T(&stream, node));
            else if (db_link_entry_T::recognise_item(line))
                node->entries.push_back(new db_link_entry_T(&stream, node));
            else if (db_entry_T::recognise_item(line))
                node->entries.push_back(new db_entry_T(&stream, node));
            else
            {
                delete node;
                throw item_not_recognised_E();
            }

            this->push_back(node);
        }
    }
}

void
db_T::dump(std::ostream &stream)
{
    /* dump our entries */
    entries_type::iterator e;
    for (e = this->entries.begin() ; e != this->entries.end() ; ++e)
    {
        (*e)->dump(stream);
        stream << this->_indent_str << "end" << std::endl;
    }

    /* then dump all our child nodes */
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->dump(stream);
}

void
db_T::display(std::ostream &stream)
{
    /* display our entries */
    entries_type::iterator e;
    for (e = this->entries.begin() ; e != this->entries.end() ; ++e)
        (*e)->display(stream);

    /* then display all our child nodes */
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->display(stream);
}

void
db_T::do_export(std::ostream &stream)
{
    /* export our entries */
    entries_type::iterator e;
    for (e = this->entries.begin() ; e != this->entries.end() ; ++e)
        (*e)->do_export(stream);

    /* then export all our child nodes */
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        (*i)->do_export(stream);
}

/*
 * Return the first entry found with the specified id,
 * or the first entry if id is empty.
 */

static bool
entry_is(db_T::entry_type *e, util::string id) { return e->is(id); }

db_T::entry_type *
db_T::entry(const util::string &id)
{
    if (this->entries.empty())
        return NULL;

    if (id.empty())
        return this->entries.front();

    entries_type::iterator i = std::find_if(this->entries.begin(),
            this->entries.end(), std::bind2nd(std::ptr_fun(entry_is), id));
    
    return (i == this->entries.end() ? NULL : *i);
}

/*
 * Tidy up our entries.
 */

db_T::~db_T()
{
    entries_type::iterator i;
    for (i = this->entries.begin() ; i != this->entries.end() ; ++i)
        delete *i;
}

/* vim: set tw=80 sw=4 et : */

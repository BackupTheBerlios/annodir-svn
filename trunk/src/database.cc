/*
 * annodir -- src/database.cc
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

#include "src/database.hh"
#include "src/database_entry.hh"
#include "src/database_note_entry.hh"
#include "src/database_link_entry.hh"
#include "src/exceptions.hh"

/*
 * Create a new database_T instance
 */
database_T::database_T()
{

}

/*
 * Create a new database_T instance, and populate it with items read from the
 * stream supplied.
 */
database_T::database_T(std::istream &stream)
{
    load(stream);
}

    void
database_T::load(std::istream &stream)
{
    while (! stream.eof())
    {
        /* get the header to determine what to create */
        std::string s;
        if (std::getline(stream, s))
        {
            database_entry_T *entry;

            /* strip trailing colon */
            if (s[s.length() - 1] != ':')
                throw item_not_parsable_E();
            s.erase(s.length() - 1);

            /* try to find a relevant class */
            if (database_note_entry_T::recognise_item(s))
                entry = new database_note_entry_T(&stream);
            else if (database_link_entry_T::recognise_item(s))
                entry = new database_link_entry_T(&stream);
            else if (database_entry_T::recognise_item(s))
                entry = new database_entry_T(&stream);
            else
                throw item_not_recognised_E();

            entries.push_back(entry);
        }
    }
}

/*
 * Dump our data (including our entries) to the supplied output stream
 */
    bool
database_T::dump(std::ostream &stream)
{
    std::vector<database_entry_T * >::iterator i;
    for (i = entries.begin() ; i != entries.end() ; ++i)
    {
        if (! (*i)->dump(stream))
            return false;
    }
    return true;
}

/*
 * Display our data (including our entries) to the supplied output stream
 */
    void
database_T::display(std::ostream &stream)
{
    std::vector<database_entry_T * >::iterator i;
    for (i = entries.begin() ; i != entries.end() ; ++i)
        (*i)->display(stream);
}

/*
 * Tidy up. Delete all of our entries.
 */
database_T::~database_T()
{
    std::vector<database_entry_T * >::iterator i;
    for (i = entries.begin() ; i != entries.end() ; ++i)
        delete *i;
}

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/anno_db.cc
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

#include "src/anno_db.hh"
#include "src/anno_db_entry.hh"
#include "src/anno_db_note_entry.hh"
#include "src/exceptions.hh"

/*
 * Create a new anno_db_T instance
 */
anno_db_T::anno_db_T()
{

}

/*
 * Create a new anno_db_T instance, and populate it with items read from the
 * stream supplied.
 */
anno_db_T::anno_db_T(std::istream &stream)
{
    load(stream);
}

    void
anno_db_T::load(std::istream &stream)
{
    while (! stream.eof())
    {
        /* get the header to determine what to create */
        std::string s;
        if (std::getline(stream, s))
        {
            anno_db_entry_T *entry;

            /* strip trailing colon */
            if (s[s.length() - 1] != ':')
                throw item_not_parsable_E();
            s.erase(s.length() - 1);

            /* try to find a relevant class */
            if (anno_db_note_entry_T::recognise_item(s))
                entry = new anno_db_note_entry_T(&stream);
            else if (anno_db_entry_T::recognise_item(s))
                entry = new anno_db_entry_T(&stream);
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
anno_db_T::dump(std::ostream &stream)
{
    std::vector<anno_db_entry_T * >::iterator i;
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
anno_db_T::display(std::ostream &stream)
{
    std::vector<anno_db_entry_T * >::iterator i;
    for (i = entries.begin() ; i != entries.end() ; ++i)
        (*i)->display(stream);
}

/*
 * Tidy up. Delete all of our entries.
 */
anno_db_T::~anno_db_T()
{
    std::vector<anno_db_entry_T * >::iterator i;
    for (i = entries.begin() ; i != entries.end() ; ++i)
        delete *i;
}

/* vim: set tw=80 sw=4 et : */

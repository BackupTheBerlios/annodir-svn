/*
 * annodir -- src/db_note_entry.cc
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

#include "db_note_entry.hh"

void
db_note_entry_T::display(std::ostream &stream)
{
    stream << this->_mynode->indent() << this->_mynode->index() << ". "
        << this->keys.get_with_default("title", "Untitled");

    if (not optget("summarise", bool))
    {
        if (optget("compact", bool) and not this->keys["body"].empty())
            stream << ": ";
        else if (not optget("compact", bool) and not this->keys["body"].empty())
            stream << std::endl << this->_mynode->indent()
                << this->_mynode->indent() << this->_padding;

        stream << this->keys.get_with_default("body", "(no text)");

        if (optget("verbose", bool))
        {
            util::string date("(no date)");
            {
                entry_keys_T::iterator pos = this->keys.find("created_at");
                if (pos != this->keys.end())
                    date.assign(util::format_date(pos->second));
            }

            if (optget("compact", bool))
            {
                stream << " ["
                    << this->keys.get_with_default("created_by", "(anonymous)")
                    << ", " << date << "]";
            }
            else
            {
                stream << std::endl << this->_mynode->indent()
                    << this->_mynode->indent()
                    << this->_padding << "Created by "
                    << this->keys.get_with_default("created_by", "(anonymous)")
                    << ", " << date;
            }
        }
    }

    stream << std::endl;
}

bool
db_note_entry_T::prompt_user_for_values()
{
    if (not db_entry_T::prompt_user_for_values())
        return false;

    /* TODO: finish me */

    return true;
}

/* vim: set tw=80 sw=4 et : */

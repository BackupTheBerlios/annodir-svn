/*
 * annodir -- src/anno_db_note_entry.cc
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

#include "src/anno_db_note_entry.hh"

/*
 * Create a new item read from the supplied stream.
 */
anno_db_note_entry_T::anno_db_note_entry_T(std::istream &stream)
{
    id = default_id();
    load(stream);
}

/*
 * Create a new item
 */
anno_db_note_entry_T::anno_db_note_entry_T()
{
    id = default_id();
}

/*
 * Get the default id for an object of our kind
 */
    std::string
anno_db_note_entry_T::default_id()
{
    return "note";
}

/*
 * Do we recognise this item?
 */
    bool
anno_db_note_entry_T::recognise_item(std::string item)
{
    return (item == "note");
}

/* vim: set tw=80 sw=4 et : */

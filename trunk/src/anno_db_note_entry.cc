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
#include "src/options.hh"
#include <cstdlib>

/*
 * Create a new item read from the supplied stream.
 */
anno_db_note_entry_T::anno_db_note_entry_T(std::istream *stream)
    : anno_db_entry_T(stream)
{
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
 * Display pretty output
 */
    void
anno_db_note_entry_T::display(std::ostream &stream)
{
    options_T options;

    stream << "[note] "
        << keys.get_with_default("title", "Untitled");

    if (!options.summarise())
    {
        if (options.compact())
            stream << ": ";
        else
            stream << std::endl << "  ";

        stream
            << keys.get_with_default("body", "(no text)");

        if (options.verbose())
        {
            std::string date_str = "(no date)";
            {
                anno_db_entry_keys_T::iterator pos = keys.find("created_at");
                if (keys.end() != pos)
                {
                    time_t date_time_t = strtol(pos->second.c_str(), NULL, 10);
                    if (0 != date_time_t)
                    {
                        char buf[255] = { 0 };
                        strftime(buf, sizeof(buf), "%x", localtime(&date_time_t));
                        date_str.assign(buf);
                    }
                }
            }

            if (options.compact())
            {
                stream << " [" << keys.get_with_default("created_by", "(anonymous)");
                stream << ", " << date_str;
                stream << "]";
            }
            else /* !compact */
            {
                stream << std::endl;
                stream << "  Created by " << keys.get_with_default("created_by", "(anonymous)");
                stream << ", " << date_str;
            } /* if !compact */

        } /* if !verbose */
    } /* if !summarise */

    stream << std::endl;
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

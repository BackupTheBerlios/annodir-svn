/*
 * annodir -- src/database_note_entry.cc
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

#include "src/database_note_entry.hh"
#include "src/options.hh"
#include "src/input.hh"
#include <cstdlib>

/*
 * Create a new item read from the supplied stream.
 */
database_note_entry_T::database_note_entry_T(std::istream *stream)
    : database_entry_T(stream)
{
    id = default_id();
}

/*
 * Get the default id for an object of our kind
 */
    std::string
database_note_entry_T::default_id()
{
    return "note";
}

/*
 * Display pretty output
 */
    void
database_note_entry_T::display(std::ostream &stream)
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
                database_entry_keys_T::iterator pos = keys.find("created_at");
                if (keys.end() != pos)
                {
                    time_t date_time_t = strtol(pos->second.c_str(), NULL, 10);
                    if (0 != date_time_t)
                    {
                        char buf[255] = { 0 };
                        /* gcc warning on the following line is spurious */
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
database_note_entry_T::recognise_item(std::string item)
{
    return (item == "note");
}

    void
database_note_entry_T::set_new_object_defaults()
{
    database_entry_T::set_new_object_defaults();
}

    bool
database_note_entry_T::prompt_user_for_values()
{
    if (!database_entry_T::prompt_user_for_values())
        return false;

    char *input = NULL;
    if (!((input = get_user_input("Title"))))
        return false;
    keys["title"].assign(input);

    if (!((input = get_user_input("Body"))))
        return false;
    keys["body"].assign(input);

    return true;
}

/* vim: set tw=80 sw=4 et : */

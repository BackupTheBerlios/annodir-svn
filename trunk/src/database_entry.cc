/*
 * annodir -- src/database_entry.cc
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

#include "src/database_entry.hh"
#include "src/options.hh"
#include <time.h>

    std::string&
database_entry_keys_T::get_with_default(std::string key,
        std::string default_value)
{
    database_entry_keys_T::iterator pos;
    pos = this->find(key);
    if (pos != this->end())
        return pos->second;
    else
        return default_value;
}

/*
 * Load from stream
 */
    void
database_entry_T::load(std::istream &stream)
{
    std::string s;
    while ((std::getline(stream, s)) && (s != "end"))
    {
        /* strip leading whitespace */
        std::string::size_type start_pos;
        if (std::string::npos != (start_pos = s.find_first_not_of(" \t")))
            s.erase(0, start_pos);

        /* split on = */
        std::string::size_type equals_pos;
        if (std::string::npos != (equals_pos = s.find('=')))
            keys[s.substr(0, equals_pos)] = s.substr(equals_pos + 1);
        else
            keys[s] = "undefined";
    }
}

/*
 * Create a new item read from the supplied stream.
 */
database_entry_T::database_entry_T(std::istream *stream)
{
    id = default_id();
    if (stream)
        load(*stream);
    else
        set_new_object_defaults();
}

/*
 * Create nice default settings if an item is newly created.
 */
        void
database_entry_T::set_new_object_defaults()
{
    options_T options;

#ifdef HAVE_ASPRINTF
    char *str;
    asprintf(&str, "%d", time(NULL));
#else
    char str[255] = { 0 };
    snprintf(str, sizeof(str) - 1, "%d", time(NULL));
#endif

    keys["created_by"].assign(options.get_user());
    keys["created_at"].assign(str);

#ifdef HAVE_ASPRINTF
    free(str);
#endif
}

    bool
database_entry_T::prompt_user_for_values()
{

}

/*
 * Dump our data to the supplied stream.
 */
    bool
database_entry_T::dump(std::ostream &stream)
{
    /* block header */
    stream << id << ":" << std::endl;

    /* entries */
    std::map<std::string, std::string >::iterator i;
    for (i = keys.begin() ; i != keys.end() ; ++i)
    {
        stream << "  " << i->first << "=" << i->second << std::endl;
    }

    /* end */
    stream << "end" << std::endl;

    return true;
}

/*
 * Display our data
 */
    void
database_entry_T::display(std::ostream &stream)
{
    /* block header */
    stream << id << ":" << std::endl;

    /* entries */
    database_entry_keys_T::iterator i;
    for (i = keys.begin() ; i != keys.end() ; ++i)
    {
        stream << "  " << i->first << "=" << i->second << std::endl;
    }
}

/*
 * Get the default id for an object of our kind
 */
    std::string
database_entry_T::default_id()
{
    return "item";
}

/*
 * Do we recognise this item? Always return true, since we can be used as a
 * fallback if necessary.
 */
    bool
database_entry_T::recognise_item(std::string item)
{
    return true;
}
/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/database_entry.cc
 * $Id$
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

#include "src/database_entry.hh"
#include "src/node_entry.hh"
#include "src/database_note_entry.hh"
#include "src/database_link_entry.hh"
#include "src/options.hh"
#include "src/exceptions.hh"
#include "src/util.hh"

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
 * Create a new entry (belonging to the specified node)
 */
database_entry_T::database_entry_T(node_entry_T *node)
{
    id = default_id();
    if (node)
        mynode = node;
}

/*
 * Create a new entry (belonging to the specified node)
 * read from the supplied stream.
 */
database_entry_T::database_entry_T(std::istream *stream, node_entry_T *node)
{
    id = default_id();
    if (node)
        mynode = node;
    if (stream)
        load(*stream);
    else
        set_new_object_defaults();
}

/*
 * Load from stream
 */
    void
database_entry_T::load(std::istream &stream)
{
    std::string s;
    while (std::getline(stream, s))
    {
        util::debug_msg("db_entry_T::load(): %s", s.c_str());

        /* strip leading whitespace */
        std::string::size_type start_pos;
        if (std::string::npos != (start_pos = s.find_first_not_of(" \t")))
            s.erase(0, start_pos);

        /* sub entry? */
        if (s[s.length() - 1] == ':')
        {
            s.erase(s.length() - 1);
            node_entry_T *node = new node_entry_T(mynode);
            
            /* try to find a relevant class */
            if (database_note_entry_T::recognise_item(s))
                node->entry = new database_note_entry_T(&stream, node);
            else if (database_link_entry_T::recognise_item(s))
                node->entry = new database_link_entry_T(&stream, node);
            else if (recognise_item(s))
                node->entry = new database_entry_T(&stream, node);
            else
                throw item_not_recognised_E();

            mynode->children.push_back(node);
        }

        if (s == "end")
            break;

        /* split on = */
        std::string::size_type equals_pos;
        if (std::string::npos != (equals_pos = s.find('=')))
            keys[s.substr(0, equals_pos)] = s.substr(equals_pos + 1);
        else
            keys[s] = "undefined";
    }
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
    asprintf(&str, "%lu", (unsigned long) time(NULL));
#else
    char str[255] = { 0 };
    snprintf(str, sizeof(str) - 1, "%lu", (unsigned long) time(NULL));
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
    return true;
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

    /* loop through children */
    std::vector<node_entry_T * >::iterator x;
    for (x = mynode->children.begin() ; x != mynode->children.end() ; ++x)
    {
        if (! (*x)->entry->dump(stream))
            return false;
    }
    return true;
}

/*
 * Display our data
 */
    void
database_entry_T::display(std::ostream &stream)
{
    if (id != "metadata")
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

    /* loop through children */
    std::vector<node_entry_T * >::iterator x;
    for (x = mynode->children.begin() ; x != mynode->children.end() ; ++x)
        (*x)->entry->display(stream);
}

/*
 * Export our data (in a todo-like list) to the specified stream
 */
    void
database_entry_T::do_export(std::ostream &stream)
{
    if (id == "metadata")
        stream << "[" << keys["title"] << "] " << std::endl; 
    else
        stream << "[" << id << "] " 
            << keys.get_with_default("title", "Untitled") << std::endl;

    if (!keys["body"].empty())
        stream << keys.get_with_default("body", "(no text)") << std::endl;

    stream << "created by: " << keys.get_with_default("created_by",
            "(anonymous)");
    
    std::string date_str = "(no date)";
    {
        database_entry_keys_T::iterator pos = keys.find("created_at");
        if (keys.end() != pos)
            date_str = util::format_datestr(pos->second);
    }

    stream << " on: " << date_str;

    if(!keys["priority"].empty())
        stream << " with priority: "
            << keys.get_with_default("priority", "medium");

    stream << std::endl << std::endl;

    /* loop through children */
    std::vector<node_entry_T * >::iterator i;
    for (i = mynode->children.begin() ; i != mynode->children.end() ; ++i)
        (*i)->entry->do_export(stream);
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

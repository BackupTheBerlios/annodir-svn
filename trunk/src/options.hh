/*
 * annodir -- src/options.hh
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

#include "config.h"

#ifndef HAVE_OPTIONS_HH
#define HAVE_OPTIONS_HH 1

#include <string>

enum options_action_T
{
    action_unspecified,
    action_list,
    action_add,
    action_edit,
    action_delete,
    action_export
};

class options_T
{
    protected:
        static std::string _file;
        static std::string _user;
        static std::string _type;
        static std::string _export;
        static bool _verbose;
        static bool _compact;
        static bool _summarise;
        static bool _recursive;
        static options_action_T _action;

    public:
        void set_verbose(bool value) { options_T::_verbose = value; }
        bool verbose() { return options_T::_verbose; }

        void set_compact(bool value) { options_T::_compact = value; }
        bool compact() { return options_T::_compact; }

        void set_summarise(bool value) { options_T::_summarise = value; }
        bool summarise() { return options_T::_summarise; }

        void set_recursive(bool value) { options_T::_recursive = value; }
        bool recursive() { return options_T::_recursive; }

        void set_action(options_action_T value) { options_T::_action = value; }
        options_action_T action() { return options_T::_action; }

        void set_filename(std::string const &value) { options_T::_file = value; }
        std::string const &get_filename() { return options_T::_file; }

        void set_user(std::string const &value) { options_T::_user = value; }
        std::string const &get_user() { return options_T::_user; }

        void set_type(std::string const &value) { options_T::_type = value; }
        std::string const &get_type() { return options_T::_type; }

        void set_export(std::string const &value) { options_T::_export = value; }
        std::string const &get_export() { return options_T::_export; }
};

#endif

/* vim: set tw=80 sw=4 et : */

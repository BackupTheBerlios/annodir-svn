/*
 * annodir -- src/options.hh
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

#include "config.h"

#ifndef HAVE_OPTIONS_HH
#define HAVE_OPTIONS_HH 1

enum options_action_T
{
    action_unspecified,
    action_list,
    action_add,
    action_edit,
    action_delete
};

class options_T
{
    protected:
        static bool _verbose;
        static bool _recursive;
        static options_action_T _action;

    public:
        void set_verbose(bool value) { options_T::_verbose = value; }
        bool verbose() { return options_T::_verbose; }

        void set_recursive(bool value) { options_T::_recursive = value; }
        bool recursive() { return options_T::_recursive; }

        void set_action(options_action_T value) { options_T::_action = value; }
        options_action_T action() { return options_T::_action; }
};

#endif

/* vim: set tw=80 sw=4 et : */

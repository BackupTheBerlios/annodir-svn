/*
 * annodir -- src/options,cc
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

#include "config.h"
#include "src/options.hh"

bool options_T::_verbose = false;
bool options_T::_compact = false;
bool options_T::_summarise = false;
bool options_T::_recursive = false;
bool options_T::_debug = false;
bool options_T::_delete_on_empty = false;
options_action_T options_T::_action = action_unspecified;
std::string options_T::_file = ".annodir";
std::string options_T::_user = "anonymous";
std::string options_T::_type = "note";
std::string options_T::_export = "annotations";
std::string options_T::_index = "";

/* vim: set tw=80 sw=4 et : */

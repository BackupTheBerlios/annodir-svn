/*
 * annodir -- src/exceptions.hh
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

#ifndef HAVE_EXCEPTIONS_HH
#define HAVE_EXCEPTIONS_HH 1

#include "config.h"
#include <exception>

/* base exception type */
class annodir_base_E              : public std::exception { };

/* commandline handling exceptions. some of these aren't exactly exceptions as
 * such, but it's easier to treat them that way. */
class args_E                      : public annodir_base_E { };
class args_usage_E                : public args_E { };
class args_one_action_only_E      : public args_usage_E { };
class args_help_E                 : public args_E { };
class args_version_E              : public args_E { };
class args_unimplemented_E        : public args_E { };

/* annodir file exceptions */
class annodir_file_E              : public annodir_base_E { };
class annodir_file_unreadable_E   : public annodir_file_E { };
class annodir_file_unwriteable_E  : public annodir_file_E { };
class annodir_file_notthere_E     : public annodir_file_E { };

/* item exceptions */
class item_E                      : public annodir_base_E { };
class item_not_recognised_E       : public item_E { };
class item_not_parsable_E         : public item_E { };

#endif

/* vim: set tw=80 sw=4 et : */

/*
 * annodir -- src/exceptions.hh
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

#ifndef HAVE_EXCEPTIONS_HH
#define HAVE_EXCEPTIONS_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdexcept>
#include "util/exceptions.hh"

/* base exceptions */
class annodir_base_E                : public util::base_E { };
class annodir_base_msg_E            : public util::msg_base_E,
                                      public annodir_base_E
{
    protected:
        const char *str;

    public:
        annodir_base_msg_E() { }
        annodir_base_msg_E(const util::string &msg) : util::msg_base_E(msg) { }
        virtual const char *what() const throw() { return this->str; }
};

class annodir_base_bad_cast_E       : public std::bad_cast,
                                      public annodir_base_E { };
class annodir_base_errno_E          : public util::errno_E,
                                      public annodir_base_E
{
    public:
        annodir_base_errno_E() { }
        annodir_base_errno_E(const util::string &msg)
            : util::errno_E(msg) { }
        virtual const char *what() const throw()
        { return util::errno_E::what(); }
};

/* file exceptions */
class annodir_bad_file_E            : public annodir_base_errno_E
{
    public:
        annodir_bad_file_E() { }
        annodir_bad_file_E(const util::string &msg)
            : annodir_base_errno_E(msg) { }
};

/* command-line handling exceptions */
class args_E                        : public annodir_base_E { };
class args_usage_E                  : public args_E { };
class args_one_action_only_E        : public args_usage_E { };
class args_help_E                   : public args_E { };
class args_version_E                : public args_E { };
class args_unimplemented_E          : public args_E { };

/* option exceptions */
class bad_option_cast_E             : public annodir_base_bad_cast_E { };
class invalid_option_E              : public annodir_base_msg_E
{
    public:
        invalid_option_E(const util::string &msg) : annodir_base_msg_E(msg) { }
        virtual const char *what() const throw()
        {
            return util::sprintf("Invalid option '%s'", str).c_str();
        }
};

/* item exceptions */
class item_E                        : public annodir_base_E { };
class item_not_recognised_E         : public item_E { };
class item_not_parsable_E           : public item_E { };

/* node exceptions */
class node_E                        : public annodir_base_E { };
class node_invalid_index_E          : public node_E,
                                      public annodir_base_msg_E
{
    protected:
        const char *str;

    public:
        node_invalid_index_E(const util::string &msg)
            : annodir_base_msg_E(msg), str(msg.c_str()) { }
        virtual const char *what() const throw()
        {
            return util::sprintf("Invalid index '%s'", str).c_str();
        }
};
class node_invalid_parent_E         : public node_E { };
class node_only_one_index_E         : public node_E { };

#endif

/* vim: set tw=80 sw=4 et : */

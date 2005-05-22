/*
 * annodir -- src/db_meta_entry.cc
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "db.hh"
#include "db_meta_entry.hh"

void
db_meta_entry_T::set_new_object_defaults()
{
    db_entry_T::set_new_object_defaults();

    const util::string pwd(util::getcwd());
    this->keys["title"] = util::basename(pwd);
    this->keys["location"] = pwd;
}

//void
//db_meta_entry_T::dump(std::ostream &stream)
//{
//    stream << this->_mynode->indent() << this->_id << ":" << std::endl;

//    entry_keys_T::iterator i;
//    for (i = this->keys.begin() ; i != this->keys.end() ; ++i)
//        stream << this->_mynode->indent() << "  " << i->first
//            << "=" << i->second << std::endl;
//}

void
db_meta_entry_T::do_export(std::ostream &stream)
{
    stream << "[" << this->keys["title"] << "] " << std::endl;
    db_entry_T::do_export(stream);
}

/* vim: set tw=80 sw=4 et : */

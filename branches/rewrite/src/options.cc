/*
 * annodir -- src/options.cc
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

#include <locale>
#include <utility>

#include "options.hh"
#include "util/string.hh"

options_T::option_map_T options_T::optmap;

#define insopt(k,v) insert(std::make_pair(k, new option_type_T(v)))

/*
 * Set default options here.
 * This is the only place where new options need to be added.
 */

void
options_T::option_map_T::set_defaults()
{
    /* booleans */
    insopt("verbose", false);
    insopt("compact", false);
    insopt("summarize", false);
    insopt("debug", false);
    insopt("recursive", false);

    /* numerics */
    insopt("maxcol", 78);

    /* other */
    insopt("dbfile", util::string(".annodir"));
    insopt("exportfile", util::string("annotations"));
    insopt("user", util::string("anonymous"));
    insopt("action", static_cast<options_action_T>(action_unspecified));
    insopt("outfile", util::string("stdout"));
    insopt("outstream", &std::cout);

    insopt("rcfile", util::string(SYSCONFDIR) + "/annodirrc");
    insopt("locale", util::string(std::locale::classic().name()));
}

/* vim: set tw=80 sw=4 et : */

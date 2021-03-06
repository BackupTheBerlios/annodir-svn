/*
 * annodir -- src/util.hh
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
#ifndef HAVE_UTIL_HH
#define HAVE_UTIL_HH 1

#include <string>
#include <cstdarg>
#include <unistd.h>

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

namespace util
{
    const char * basename(const char *);
    const char * basename(std::string const &);
    const char * dirname(const char *);
    const char * dirname(std::string const &);
    std::string format_datestr(std::string const &);
    std::string sprintf(const char *, ...);
    std::string sprintf(const char *, va_list);
    void debug_msg(const char *, ...);
}

#endif

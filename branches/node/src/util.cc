/*
 * annodir -- src/util.cc
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
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>

#include "src/util.hh"
#include "src/options.hh"

    char *
util::basename(char *path)
{
    char *s = std::strrchr(path, '/');
    return (s ? s + 1 : path);
}

    std::string
util::format_datestr(std::string& epoch)
{
    std::string date_str;
    std::time_t date_time_t = std::strtol(epoch.c_str(), NULL, 10);

    if (0 != date_time_t)
    {
	char buf[255] = { 0 };
	/* gcc warning on the following line is spurious.
	   (doesn't seem to happen w/3.4.x though. -ka0ttic) */
	std::strftime(buf, sizeof(buf), "%x", std::localtime(&date_time_t));
	date_str.assign(buf);
    }
    return (date_str.empty() ? "(no date)" : date_str);
}

/*
 * Return a string formatted with printf-like format specifier
 */
    std::string
util::sprintf(const char *str, ...)
{
    va_list v;
    va_start(v, str);
    std::string s(util::sprintf(str, v));
    va_end(v);
    return s;
}

/*
 * Overloaded sprintf that takes a string and va_list
 */
    std::string
util::sprintf(const char *str, va_list v)
{
#ifdef HAVE_VASPRINTF
    char *buf;
    vasprintf(&buf, str, v);
#else
    char buf[4096] = { 0 };
# ifdef HAVE_VSNPRINTF
    vsnprintf(buf, sizeof(buf), str, v);
# else
    vsprintf(buf, str, v);
# endif
#endif

    std::string s(buf);

#ifdef HAVE_VASPRINTF
    std::free(buf);
#endif

    return s;
}

/*
 * I hate to bring in options_T just for this, but it's either here
 * or make the caller declare an options_T instance and pass a bool value
 */
    void
util::debug_msg(const char *msg, ...)
{
    options_T options;

    if (not options.debug())
	return;
    
    va_list v;
    va_start(v, msg);
    std::cout << util::sprintf(msg, v) << std::endl;
    va_end(v);
}

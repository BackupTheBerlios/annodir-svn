/*
 * annodir -- src/util.cc
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
#include "src/util.hh"

#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>

    const char *
basename(char *path)
{
    path = (path += strlen(path));
    while(*(--path) != '/');
    return ++path;
}

    std::string
format_datestr(std::string& epoch)
{
    std::string date_str;
    time_t date_time_t = strtol(epoch.c_str(), NULL, 10);

    if (0 != date_time_t)
    {
	char buf[255] = { 0 };
	/* gcc warning on the following line is spurious.
	   (doesn't seem to happen w/3.4.x though. -ka0ttic) */
	strftime(buf, sizeof(buf), "%x", localtime(&date_time_t));
	date_str.assign(buf);
    }
    return (date_str.empty() ? "(no date)" : date_str);
}

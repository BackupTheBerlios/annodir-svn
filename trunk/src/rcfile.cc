/*
 * annodir -- src/rcfile.cc
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
#include "src/rcfile.hh"
#include <string>
#include <iostream>

rcfile_T::rcfile_T(std::ifstream &file)
{
    /* rcfile format is key=value, comment lines start with # */
    std::string line;
    std::string::size_type pos;
    while (std::getline(file, line))
    {
        /* kill whitespace */
        if (std::string::npos != (pos = line.find_first_not_of(" \t")))
            line.erase(0, pos);

        if (line.length() < 1)
            continue;

        if (line.at(0) == '#')
            continue;

        if (std::string::npos != (pos = line.find('=')))
        {
            std::string key(line.substr(0, pos));
            std::string val(line.substr(pos + 1));

            /* kill spaces */
            if (std::string::npos != (pos = key.find_first_not_of(" \t")))
                key.erase(0, pos);
            if (std::string::npos != (pos = val.find_first_not_of(" \t")))
                val.erase(0, pos);
            if (std::string::npos != (pos = key.find_last_not_of(" \t")))
                key.erase(++pos);
            if (std::string::npos != (pos = val.find_last_not_of(" \t")))
                val.erase(++pos);

            keys[key] = val;
        }
        else
        {
            keys[line] = "";
        }
    }
}

/* vim: set tw=80 sw=4 et : */

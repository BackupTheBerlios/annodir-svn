/*
 * annodir -- src/input.cc
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
#include "src/input.hh"
#include <string>

/* needed for readline... */
#include <cstdlib>
#include <cstdio>
#include <readline/readline.h>

std::string const *rl_buffer;

    static int
init_readline()
{
    rl_insert_text(const_cast<char * >(rl_buffer->c_str()));
    return 0;
}

    char *
input::get_user_input(std::string const &prompt, std::string existing_text)
{
    std::string pretty_prompt(prompt);
    while (pretty_prompt.length() < 8)
        pretty_prompt.append(" ");
    pretty_prompt.append(" > ");

    rl_startup_hook = (int(*)())init_readline;
    rl_buffer = &existing_text;
    return readline(pretty_prompt.c_str());
}

/* vim: set tw=80 sw=4 et : */

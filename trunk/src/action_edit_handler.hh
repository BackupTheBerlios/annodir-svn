/*
 * annodir -- src/action_edit_handler.hh
 * $Id: node_entry.hh 92 2004-12-10 11:35:38Z ka0ttic $
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

#ifndef ACTION_EDIT_HANDLER_HH
#define ACTION_EDIT_HANDLER_HH 1

#include "src/action_handler.hh"

class action_edit_handler_T : public action_handler_T
{
    public:
	action_edit_handler_T() { };
	virtual int operator() (void);
};

#endif

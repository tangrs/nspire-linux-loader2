/*
    TI-NSPIRE Linux In-Place Bootloader
    Copyright (C) 2012  Daniel Tang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MACROS_H
#define MACROS_H

#include <nspireio.h>
#include "common.h"

#define ROUND_PAGE_BOUND(x) ((typeof(x))((unsigned)(x)&~(PAGE_SIZE-1)))
#define ROUND_UP_PAGE_BOUND(x) ((typeof(x))((unsigned)(x+PAGE_SIZE-1)&~(PAGE_SIZE-1)))

#define printl nio_printf

#endif
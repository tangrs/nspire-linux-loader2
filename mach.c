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

#include "common.h"

/*
    Should detect which calc model and write an appropriate machine number
    for booting Linux.

    return negative value for error
*/
int detect_machine() {
    /* Placeholder */
    settings.machine_id = 3503;
    return 0;
}

/*
    Should (non-destructively) detect memory and fill in settings.phys
    return negative value for error
*/
int detect_memory() {
    /* Placeholder */
    settings.phys.start = (void*)0x10000000;
    settings.phys.size  = 0x4000000;
    return 0;
}
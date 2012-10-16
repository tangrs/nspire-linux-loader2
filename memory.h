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


#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

#define MAX_ATAG_SIZE           (0x8000-0x100)
#define mem_block_size_free() (settings.mem_block.size - settings.kernel.size - settings.ramdisk.size)


void alloc_memory();
void free_memory();
void show_mem(char*);

#endif
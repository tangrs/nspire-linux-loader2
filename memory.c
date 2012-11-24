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

#include <os.h>
#include "common.h"
#include "memory.h"
#include "macros.h"

/* Amount of memory to try extending each time in max_malloc */
#define INC_BLOCK_SIZE 0x10000

/*
    Try to alloc the biggest contigious section of memory possible.
    Will always be a multiple of INC_BLOCK_SIZE
*/
static void* max_malloc(size_t *size) {
    void * ptr = NULL;
    size_t curr_size = 0;

    while(1) {
        size_t new_size = curr_size + INC_BLOCK_SIZE;
        void *tmp = realloc(ptr, new_size);
        if (!tmp) break;
        ptr = tmp;
        curr_size = new_size;
    }

    *size = curr_size;
    return ptr;
}

#define HUMAN(x) do { \
        char *unit; \
        unsigned number = x; \
        if      (x < 1024)        { unit = "B"; } \
        else if (x < 1024 * 1024) { unit = "K"; number /= 1024; } \
        else                      { unit = "M"; number /= 1024 * 1024; } \
        printl("%u%s", number, unit); \
    } while (0)

void show_mem(char *ignored __attribute__((unused))) {
        printl("Total free: ");
        HUMAN(mem_block_size_free());
        printl("/");
        HUMAN(settings.mem_block.size);
        printl("" NEWLINE);

        printl("Kernel:  ");
        HUMAN(settings.kernel.size);
        printl("/");
        HUMAN(settings.mem_block.size);
        printl("" NEWLINE);

        printl("Ramdisk: ");
        HUMAN(settings.ramdisk.size);
        printl("/");
        HUMAN(settings.mem_block.size);
        printl("" NEWLINE);
}

#undef HUMAN

void free_memory() {
    free(settings.atag.start);
    free(settings.mem_block.start);

    settings.atag.start = NULL;
    settings.mem_block.start = NULL;

    settings.atag.size = 0;
    settings.mem_block.size = 0;

    settings.kernel.size = 0;
    settings.kernel.addr = 0;

    settings.ramdisk.size = 0;
    settings.ramdisk.addr = 0;

    settings.kernel_loaded = 0;
    settings.ramdisk_loaded = 0;
}

void alloc_memory() {
    free_memory();

    settings.atag.start = malloc(MAX_ATAG_SIZE);
    if (settings.atag.start) settings.atag.size = MAX_ATAG_SIZE;

    /* Allocate as much remaining memory as possible */
    settings.mem_block.start = max_malloc(&settings.mem_block.size);

    printl( "Allocated memory:" NEWLINE
            "   ATAGs:                 %u bytes" NEWLINE
            "   Kernel and ramdisk:    %u bytes" NEWLINE,
            settings.atag.size, settings.mem_block.size);
}

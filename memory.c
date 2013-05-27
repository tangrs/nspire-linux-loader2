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

/* Amount of memory to try extending each time in max_malloc */
#define INC_BLOCK_SIZE 0x10000

/*
    Try to alloc the biggest contigious section of memory possible.
    Will always be a multiple of INC_BLOCK_SIZE
*/
static void *max_malloc(size_t *size) {
    void *ptr = NULL;
    size_t curr_size = 0;

    while(1) {
        size_t new_size = curr_size + INC_BLOCK_SIZE;
        void *tmp = realloc(ptr, new_size);
        if (!tmp) break;
        ptr = tmp;
        curr_size = new_size;
    }

    free(ptr);

    /* Reserve some space for FDT */
    curr_size -= 64 * 1024; /* 64KB ought to be enough */
    ptr = malloc(curr_size);

    *size = !!ptr ? curr_size : 0;
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

void show_mem(char *ignored UNUSED) {
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

        printl("Initrd: ");
        HUMAN(settings.initrd.size);
        printl("/");
        HUMAN(settings.mem_block.size);
        printl("" NEWLINE);
}

#undef HUMAN

void free_memory() {
    free(settings.boot_param.start);
    free(settings.mem_block.start);

    settings.boot_param.start = NULL;
    settings.mem_block.start = NULL;

    settings.boot_param.size = 0;
    settings.mem_block.size = 0;

    settings.kernel.size = 0;
    settings.kernel.addr = 0;

    settings.initrd.size = 0;
    settings.initrd.addr = 0;

    settings.kernel_loaded = 0;
    settings.initrd_loaded = 0;
}

void alloc_memory() {
    free_memory();

    settings.boot_param.start = malloc(MAX_BOOT_PARAM_SIZE);
    if (settings.boot_param.start)
        settings.boot_param.size = MAX_BOOT_PARAM_SIZE;

    /* Allocate as much remaining memory as possible */
    settings.mem_block.start = max_malloc(&settings.mem_block.size);

    printl( "Allocated memory:" NEWLINE
            "   DTB/ATAGs:             %u bytes" NEWLINE
            "   Kernel and initrd:     %u bytes" NEWLINE,
            settings.boot_param.size, settings.mem_block.size);
}

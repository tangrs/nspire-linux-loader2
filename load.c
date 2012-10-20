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

#include "macros.h"
#include "memory.h"

static size_t file_size(const char *filename) {
    struct stat stats;
    if (stat(filename, &stats)) return 0;
    return stats.st_size;
}

void load_kernel(const char *filename) {
    size_t kernel_size = file_size(filename);
    size_t size_free_memory = mem_block_size_free() - settings.kernel.size;
    FILE* f;

    if (!kernel_size) {
        printl("Kernel doesn't exist or empty\n");
        return;
    }

    if (kernel_size > size_free_memory) {
        printl( "Kernel too large!\n"
                "Tried to load kernel of %u bytes into %u bytes of free space\n",
                kernel_size, size_free_memory);
        return;
    }

    f = fopen(filename, "rb");
    if (!f) {
        printl("Failed to open kernel image %s\n", filename);
        return;
    }

    settings.kernel.addr = settings.mem_block.start;
    settings.kernel.size = fread(settings.kernel.addr, 1, kernel_size, f);
    settings.kernel_loaded = !!(settings.kernel.size);

    if (settings.kernel.size != kernel_size) printl("Warning: read less data from file than expected\n");

    printl("Kernel successfully loaded\n");
    return;
}

void load_ramdisk(const char *filename) {
    FILE* f;
    size_t ramdisk_size = file_size(filename);
    size_t size_free_memory = mem_block_size_free() - settings.ramdisk.size;
    void* ramdisk_laddr = ((char*)settings.mem_block.start + settings.mem_block.size - ramdisk_size);
    ramdisk_laddr = ROUND_PAGE_BOUND(ramdisk_laddr);
    size_t needed_size = ((char*)settings.mem_block.start + settings.mem_block.size)
                        - (char*)ramdisk_laddr;

    if (!strlen(filename) && settings.ramdisk_loaded) {
        settings.ramdisk_loaded = 0;
        settings.ramdisk.addr = NULL;
        settings.ramdisk.size = 0;
        printl("Unloaded ramdisk\n");
        return;
    }

    if (!ramdisk_size) {
        printl("Ramdisk doesn't exist or empty\n");
        return;
    }

    if (needed_size > size_free_memory) {
        printl( "Ramdisk too large!\n"
                "Tried to load ramdisk needing %u bytes into %u bytes of free space\n"
                "Original ramdisk size was %u bytes\n",
                needed_size, size_free_memory, ramdisk_size);
        return;
    }

    f = fopen(filename, "rb");
    if (!f) {
        printl("Failed to open ramdisk image %s\n", filename);
        return;
    }

    settings.ramdisk.addr = ramdisk_laddr;
    settings.ramdisk.size = needed_size;

    if (fread(settings.ramdisk.addr, 1, ramdisk_size, f) != ramdisk_size)
        printl("Warning: read less data from file than expected\n");

    settings.ramdisk_loaded = !!(settings.ramdisk.size);

    printl("Ramdisk successfully loaded\n");
    return;
}
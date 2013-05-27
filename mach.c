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

#include <stdint.h>

#include "common.h"

/*
    A conservative approximate of the amount of memory
    the OS takes up in megabytes.

    Used to estimate the amount of physical memory.
*/
#define APPROX_OS_SIZE      20

static int guess_memory_size() {
    size_t mem_mb = settings.mem_block.size / 1024 / 1024;
    mem_mb += APPROX_OS_SIZE;

    printl("Warning: guessing physical memory parameters" NEWLINE);
    if      (mem_mb < 16)   return -1;
    else if (mem_mb < 32)   settings.phys.size  = 0x2000000; /* 32M */
    else if (mem_mb < 64)   settings.phys.size  = 0x4000000; /* 64M */
    else if (mem_mb < 128)  settings.phys.size  = 0x8000000; /* 128M (who knows? ;D) */
    else                    return -1;

    return 0;
}

void force_guess_memory(char *ignored UNUSED) {
    if (guess_memory_size()) printl("Failed to guess memory parameters" NEWLINE);
}

/*
    Should (non-destructively) detect memory and fill in settings.phys
    return negative value for error
*/
int detect_memory() {
    /* Placeholder */
    settings.phys.start = (void*)0x10000000;
    switch (hwtype()) {
        case 0:
            /* Clickpad/original */
            printl("Detected a non-CX" NEWLINE);
            settings.phys.size = 0x2000000;
            break;
        case 1:
            /* CX */
            printl("Detected a CX" NEWLINE);
            settings.phys.size = 0x4000000;
            break;
        default:
            return guess_memory_size();
    }
    return 0;
}

/*
    Should detect which calc model and write an appropriate machine number
    for booting Linux.

    return negative value for error
*/
int detect_machine() {
    /* Placeholder */
    switch (hwtype()) {
        case 0:
            if (!is_touchpad)   settings.machine_id = 4441;
            else                settings.machine_id = 4442;
            break;
        case 1:
            settings.machine_id = 4443;
            break;
        default:
            printl( "No machine ID for this platform." NEWLINE
                    "Defaulting to DTB machine number %d." NEWLINE,
                    DTB_MACH_ID);
            settings.machine_id = DTB_MACH_ID;
            return -1;
    }
    return 0;
}

/*
    Detect serial number and revision and write it

    return negative value for error
*/
#define ADD_BITS(v,r,x,y) do { \
        unsigned len = ((y)-(x)+1); \
        v <<= len; \
        v  |= ((r)>>(x)) & ~(~0<<len); \
    } while (0)

int detect_serialnr() {
    uint64_t serial = 0, raw_serial = *(uint64_t*)0x900A0028;

    ADD_BITS(serial, raw_serial, 33, 55);
    ADD_BITS(serial, raw_serial, 17, 31);
    ADD_BITS(serial, raw_serial, 9, 15);
    ADD_BITS(serial, raw_serial, 5, 7);
    ADD_BITS(serial, raw_serial, 3, 3);

    settings.serialnr[0] = (unsigned)serial;
    settings.serialnr[1] = (unsigned)(serial>>32);
    settings.rev = (raw_serial>>58) & 0x1F;

    return 0;
}
#undef ADD_BITS

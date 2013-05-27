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
#include <stdint.h>

#include "atag_tags.h"
#include "atag.h"
#include "common.h"

static void* atag_add(void *head, int tagid, ...) {
    va_list ap;
    va_start(ap, tagid);
    char *cmdline = NULL;

    ((struct atag*)head)->hdr.tag = tagid;

    #define SET_NEXT_VARARG(type, element) ((struct atag*)head)->u.type.element = va_arg(ap, uint32_t)
    #define SET_SIZE(x) ((struct atag*)head)->hdr.size = 2 + (sizeof(((struct atag*)head)->u.x) / sizeof(uint32_t))
    switch (tagid) {
        case ATAG_MEM:
            SET_SIZE(mem);
            SET_NEXT_VARARG(mem, size);
            SET_NEXT_VARARG(mem, start);
            break;
        case ATAG_VIDEOTEXT:
            SET_SIZE(videotext);
            SET_NEXT_VARARG(videotext, x);
            SET_NEXT_VARARG(videotext, y);
            SET_NEXT_VARARG(videotext, video_page);
            SET_NEXT_VARARG(videotext, video_mode);
            SET_NEXT_VARARG(videotext, video_cols);
            SET_NEXT_VARARG(videotext, video_ega_bx);
            SET_NEXT_VARARG(videotext, video_isvga);
            SET_NEXT_VARARG(videotext, video_points);
            break;
        case ATAG_RAMDISK:
            SET_SIZE(ramdisk);
            SET_NEXT_VARARG(ramdisk, flags);
            SET_NEXT_VARARG(ramdisk, size);
            SET_NEXT_VARARG(ramdisk, start);
            break;
        case ATAG_INITRD2:
            SET_SIZE(initrd2);
            SET_NEXT_VARARG(initrd2, start);
            SET_NEXT_VARARG(initrd2, size);
            break;
        case ATAG_SERIAL:
            SET_SIZE(serialnr);
            SET_NEXT_VARARG(serialnr, low);
            SET_NEXT_VARARG(serialnr, high);
            break;
        case ATAG_REVISION:
            SET_SIZE(revision);
            SET_NEXT_VARARG(revision, rev);
            break;
        case ATAG_VIDEOLFB:
            SET_SIZE(videolfb);
            SET_NEXT_VARARG(videolfb, lfb_width);
            SET_NEXT_VARARG(videolfb, lfb_height);
            SET_NEXT_VARARG(videolfb, lfb_depth);
            SET_NEXT_VARARG(videolfb, lfb_linelength);
            SET_NEXT_VARARG(videolfb, lfb_base);
            SET_NEXT_VARARG(videolfb, lfb_size);
            SET_NEXT_VARARG(videolfb, red_size);
            SET_NEXT_VARARG(videolfb, red_pos);
            SET_NEXT_VARARG(videolfb, green_size);
            SET_NEXT_VARARG(videolfb, green_pos);
            SET_NEXT_VARARG(videolfb, blue_size);
            SET_NEXT_VARARG(videolfb, blue_pos);
            SET_NEXT_VARARG(videolfb, rsvd_size);
            SET_NEXT_VARARG(videolfb, rsvd_pos);
            break;
        case ATAG_CORE:
        case ATAG_NONE:
            ((struct atag*)head)->hdr.size = 2;
            break;
        case ATAG_CMDLINE:
            cmdline = va_arg(ap, char*);
            ((struct atag*)head)->hdr.size = 2 + ((strlen(cmdline) + sizeof(uint32_t)) / sizeof(uint32_t));
            break;
        default:
            printl("Invalid ATAG id %d", tagid);
            return head;
    }
    size_t tag_size = (((struct atag*)head)->hdr.size * sizeof(uint32_t));

    if (tagid == ATAG_NONE) ((struct atag*)head)->hdr.size = 0;
    if (cmdline) {
        strcpy(((struct atag*)head)->u.cmdline.cmdline, cmdline);
    }
    va_end(ap);
    return (char*)head + tag_size;
}

static void* atag_begin(void *head) {
    return atag_add(head, ATAG_CORE);
}

#define ATAG(...) do { \
        if (current > (char*)settings.boot_param.start + settings.boot_param.size) { \
            printl("Internal error. ATAG buffer overrun" NEWLINE); \
            return -1; \
        } \
        current = atag_add(current, __VA_ARGS__); \
    } while (0)

int atag_build() {
    char *current;
    if (!settings.boot_param.start) {
        printl("Internal error. settings.boot_param.start = NULL" NEWLINE);
        return -1;
    }
    current = atag_begin(settings.boot_param.start);
    /*
        Begin building list of ATAGs
    */
    if (settings.phys.size)
        ATAG(ATAG_MEM, settings.phys.size, settings.phys.start);
    if (strlen(settings.kernel_cmdline))
        ATAG(ATAG_CMDLINE, settings.kernel_cmdline);
    if (settings.initrd_loaded)
        ATAG(ATAG_INITRD2, settings.initrd.addr, settings.initrd.size);
    if (settings.ramdisk_size)
        ATAG(ATAG_RAMDISK, 0, settings.ramdisk_size, 0);

    ATAG(ATAG_SERIAL, settings.serialnr[0], settings.serialnr[1]);
    ATAG(ATAG_REVISION, settings.rev);
    /*
        End list. Stop here
    */
    ATAG(ATAG_NONE);
    return 0;
}

#undef ATAG

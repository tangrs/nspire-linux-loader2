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
#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define PAGE_SIZE 0x1000

struct params {
    struct {
        void* addr;         /* Where the kernel is loaded (address should be in range of mem_block) */
        size_t size;
    } kernel;

    struct {
        void* addr;         /* Where the ramdisk is loaded (address should be in range of mem_block) */
        size_t size;
    } ramdisk;

    struct {
        void* start;        /* A separate section of memory for writing ATAGs */
        void* raddr;        /* Where the atags should be relocated to during booting */
        size_t size;
    } atag;

    struct {
        void* start;        /* Memory block for kernel and ramdisk */
        size_t size;
    } mem_block;

    struct {
        void* start;
        size_t size;
    } phys;

    char kernel_cmdline[64];

    int machine_id;

    unsigned serialnr[2];

    unsigned ramdisk_loaded:1;
    unsigned kernel_loaded:1;
};

extern struct params settings;

void dump_settings(char*);
void setget_mach(char * arg);
void setget_phys(char * arg);

#endif
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

#include <stdarg.h>
#include <stddef.h>
#include <nspireio/nspireio.h>

#define PAGE_SIZE 0x1000
#define DTB_MACH_ID 3503

#define ALIGN(p, size) ((void*)(((unsigned)p + (size - 1)) & ~(size - 1)))
#define ROUND_PAGE_BOUND(x) ((typeof(x))((unsigned)(x)&~(PAGE_SIZE-1)))
#define ROUND_UP_PAGE_BOUND(x) ((typeof(x))((unsigned)(x+PAGE_SIZE-1)&~(PAGE_SIZE-1)))

#define printl(...) do { \
        nio_printf(__VA_ARGS__); \
        uart_printf(__VA_ARGS__); \
    } while(0)

#define NEWLINE "\r\n"
#define UNUSED  __attribute__((unused))

typedef volatile uint32_t *io32_t;
typedef volatile uint16_t *io16_t;
typedef volatile uint8_t *io8_t;

struct params {
    struct {
        void *addr;         /* Where the kernel is loaded (address should be in range of mem_block) */
        size_t size;
    } kernel;

    struct {
        void *addr;         /* Where the initrd is loaded (address should be in range of mem_block) */
        size_t size;
    } initrd;

    struct {
        void *start;        /* A separate section of memory for writing ATAG and DTBs */
        size_t size;
    } boot_param;

    struct {
        void *start;        /* Memory block for kernel and initrd */
        size_t size;
    } mem_block;

    struct {
        void *start;
        size_t size;
    } phys;

    char kernel_cmdline[128];

    int machine_id;

    unsigned serialnr[2];
    unsigned rev;
    unsigned ramdisk_size;

    unsigned initrd_loaded:1;
    unsigned kernel_loaded:1;
    unsigned dtb_loaded:1;
    unsigned break_on_entry:1;
};

extern struct params settings;

void dump_settings(char *);
void setget_mach(char *arg);
void setget_phys(char *arg);
void setget_rdisksize(char *arg);

void poke(char *arg);
void peek(char *arg);

void break_on_entry(char *arg);

void *builtin_memcpy(void *dst, const void *src, size_t size);

#endif

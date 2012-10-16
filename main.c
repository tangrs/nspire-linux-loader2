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

#include <nspireio.h>
#include "common.h"
#include "macros.h"
#include "memory.h"
#include "cmd.h"
#include "mach.h"

struct params settings = {
    .kernel_ramdisk_size = DEFAULT_RAMDISK_SIZE
};

int main() {
    nio_console csl;
    nio_init(&csl,NIO_MAX_COLS,NIO_MAX_ROWS,0,0,WHITE,BLACK,TRUE);
    nio_set_default(&csl);

    printl("Linux in-place bootloader\n");
    alloc_memory();

    if (detect_machine())
        printl("Warning: Could not detect machine number!\n");
    else
        printl("Machine number: %d\n", settings.machine_id);

    if (detect_memory())
        printl("Warning: Could not detect amount of memory!\n");
    else
        printl("Physical memory at: 0x%p-0x%p\n",
            settings.phys.start, (void*)((char*)settings.phys.start + settings.phys.size));

    while (1) {
        char cmd[128];
        nio_printf("> ");
        if (nio_gets(cmd)) {
            if (process_cmd(cmd)) break;
        }
    }

    free_memory();
    return 0;
}
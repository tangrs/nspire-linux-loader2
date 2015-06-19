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
#include "cmd.h"
#include "mach.h"
#include "trap.h"

struct params settings;

#ifdef GIT_COMMIT
#define GIT_TEXT " (" GIT_COMMIT ")"
#else
#define GIT_TEXT
#endif

int main(int argc, char *argv[]) {
    nio_console csl;
    nio_init(&csl,
            NIO_MAX_COLS,NIO_MAX_ROWS,
            0,0,
            NIO_COLOR_WHITE,NIO_COLOR_BLACK,
            TRUE);
    nio_set_default(&csl);
    trap_install();

    printl("Linux in-place bootloader v2" GIT_TEXT NEWLINE);
#ifdef BUILD_DATE
    printl("Build date: " BUILD_DATE NEWLINE);
#endif

    alloc_memory();

    if (detect_machine())
        printl("Warning: Could not detect machine number!" NEWLINE);
    else
        printl("Machine number: %d" NEWLINE, settings.machine_id);

    if (detect_memory())
        printl("Warning: Could not detect amount of memory!" NEWLINE);
    else
        printl("Physical memory at: %p-%p" NEWLINE,
            settings.phys.start, (void*)((char*)settings.phys.start + settings.phys.size));

    if (detect_serialnr())
        printl("Warning: Could not get serial number!" NEWLINE);
    else
        printl("Serial number: %x%x rev%d (%s)" NEWLINE,
                settings.serialnr[1], settings.serialnr[0], settings.rev,
                settings.rev?"CAS":"Non-CAS");

    if (argc > 1) {
        load_script(argv[1]);
    }

    while (1) {
        char cmd[128];
        nio_puts("> ");
        if (nio_getsn(cmd, 127)) {
            if (process_cmd(cmd)) break;
        }
    }

    trap_remove();
    free_memory();
    nio_free(&csl);
    return 0;
}

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

#include "kernel.h"
#include "common.h"
#include "load.h"
#include "macros.h"

/*
    Returns number of filled arguments.
    Var args are all of type char**
*/
int cmd_args(char* args, unsigned max_n, ...) {
    va_list ap;
    va_start(ap, max_n);

    char *ptr = args;
    unsigned i;
    for (i=0; i<max_n && *args; i++) {
        if (*args == ' ') {
            *args = '\0';
            char **fill = va_arg(ap,char**);
            *fill = ptr;
            ptr = args + 1;
        }
        args++;
    }

    va_end(ap);
    return i;
}

/*
    Returns 1 if program should gracefully exit
    Returns 0 if further commands can be executed
*/
#define DEFINE_COMMAND(s, f)   else if (!strncmp(#s, cmd, sizeof(#s)-1)) (cmd[sizeof(#s)-1] ? f(cmd+sizeof(#s)) : f(""))
int process_cmd(char * cmd) {
    if (*cmd == '\0') return 0;
    else if (!strcmp("exit", cmd)) return 1;
    /*
        Define custom commands below
        DEFINE_COMMAND( function_name, function_to_call );
        function_to_call should accept one parameter which is
        pointer to arguments
    */
    DEFINE_COMMAND(kernel, load_kernel);
    DEFINE_COMMAND(initrd, load_ramdisk);
    DEFINE_COMMAND(dump, dump_settings);
    DEFINE_COMMAND(cmdline, kernel_cmdline);
    DEFINE_COMMAND(boot, kernel_boot);
    /*
        End command list. Do not add any more after here
    */
    else { printl("Unknown command\n"); }
    return 0;
}
#undef DEFINE_COMMAND
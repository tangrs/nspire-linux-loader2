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


static const io32_t vectaddr_begin = (io32_t)0x00000020;
static unsigned vectaddr_orig[3];

void trap_abt(void);
void trap_data_abt(void);

struct trap_regs {
    unsigned int reg[16];
    unsigned int psr;
};

void trap_enter(struct trap_regs *regs) {
    const char *mode;

    printl("Exception trapped!" NEWLINE);
    printl("r0:  %08X r1:  %08X r2:  %08X" NEWLINE,
            regs->reg[0], regs->reg[1], regs->reg[2]);
    printl("r3:  %08X r4:  %08X r5:  %08X" NEWLINE,
            regs->reg[3], regs->reg[4], regs->reg[5]);
    printl("r6:  %08X r7:  %08X r8:  %08X" NEWLINE,
            regs->reg[6], regs->reg[7], regs->reg[8]);
    printl("r9:  %08X r10: %08X r11: %08X" NEWLINE,
            regs->reg[9], regs->reg[10], regs->reg[11]);
    printl("r12: %08X sp:  %08X lr:  %08X" NEWLINE,
            regs->reg[12], regs->reg[13], regs->reg[14]);
    printl("pc:    %08X" NEWLINE, regs->reg[15]);
    printl("cpsr:  %08X" NEWLINE, regs->psr);
    switch (regs->psr & 0x1F) {
        case 0b10000:
            mode = "usr";
            break;
        case 0b10001:
            mode = "fiq";
            break;
        case 0b10010:
            mode = "irq";
            break;
        case 0b10011:
            mode = "svc";
            break;
        case 0b10111:
            mode = "abt";
            break;
        case 0b11011:
            mode = "und";
            break;
        case 0b11111:
            mode = "sys";
            break;
        default:
            mode = "???";
    }
    printl("mode:  %s" NEWLINE, mode);
    printl("bits:  %c %c %c %c %c %c %c %c %c" NEWLINE,
        (regs->psr & (1<<31)) ? 'N' : ' ',
        (regs->psr & (1<<30)) ? 'Z' : ' ',
        (regs->psr & (1<<29)) ? 'C' : ' ',
        (regs->psr & (1<<28)) ? 'V' : ' ',
        (regs->psr & (1<<27)) ? 'Q' : ' ',
        (regs->psr & (1<<24)) ? 'J' : ' ',
        (regs->psr & (1<< 7)) ? 'I' : ' ',
        (regs->psr & (1<< 6)) ? 'F' : ' ',
        (regs->psr & (1<< 5)) ? 'T' : ' ');

    printl("Press a key to reset" NEWLINE);

    wait_key_pressed();

    while (1) {
        *(io32_t)0x900A0008 = 2;
    }
}

void trap_install(void) {
    vectaddr_orig[0] = vectaddr_begin[1];
    vectaddr_orig[1] = vectaddr_begin[3];
    vectaddr_orig[2] = vectaddr_begin[4];

    vectaddr_begin[1] = (unsigned)trap_abt;
    vectaddr_begin[3] = (unsigned)trap_abt;
    vectaddr_begin[4] = (unsigned)trap_data_abt;

    clear_cache();
}

void trap_remove(void) {
    vectaddr_begin[1] = vectaddr_orig[0];
    vectaddr_begin[3] = vectaddr_orig[1];
    vectaddr_begin[4] = vectaddr_orig[2];

    clear_cache();
}

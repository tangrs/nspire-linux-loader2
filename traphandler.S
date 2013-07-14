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

.global trap_abt
.global trap_data_abt

trap_data_abt:   sub lr, lr, #4
trap_abt:        sub lr, lr, #4

    str lr, saved_regs + (15<<2)
    adr lr, saved_regs
    stm lr, {r0-r12}

    mrs lr, spsr
    str lr, saved_cpsr
    msr cpsr, lr

    str sp, saved_regs + (13<<2)
    str lr, saved_regs + (14<<2)

    msr cpsr, #0xD3

    adr r0, saved_regs
    bl trap_enter

    ldr r0, saved_cpsr
    msr spsr, r0

    adr r0, saved_regs
    ldm r0, {r0-r15}^

    saved_regs:
        .rept 16
        .word 0
        .endr
    saved_cpsr:
        .word 0

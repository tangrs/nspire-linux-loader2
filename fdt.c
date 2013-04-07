#include <libfdt.h>
#include <os.h>

#include "fdt.h"
#include "common.h"
#include "macros.h"
#include "memory.h"

static int _fdt_make_node(void * fdt, int parentoffset, const char * name) {
    int e = fdt_subnode_offset(fdt, parentoffset, name);
    if(e != FDT_ERR_NOTFOUND)
        return e;

    return fdt_add_subnode(fdt, parentoffset, name);
}

int update_fdt()
{
    static void* fdt = 0;

    if(!fdt)
        fdt = malloc(FDT_SIZE_MAX + 7);

    if(!fdt) {
        printl("Failed to allocate memory for FDT!" NEWLINE);
        return 1;
    }

    /* Has to be 64-bit aligned */
    fdt = ALIGN(fdt, 8);

    if(fdt_open_into(settings.boot_param.start, fdt, FDT_SIZE_MAX) < 0) {
        printl("Failed to open FDT!" NEWLINE);
        return 1;
    }

    int chosen = _fdt_make_node(fdt, 0, "chosen");
    if(chosen < 0) {
	printl("Failed to create 'chosen' node!" NEWLINE);
	return 1;
    }

    /* If not given assume defaults from DT */
    if(strlen(settings.kernel_cmdline) && fdt_setprop_string(fdt, chosen, "bootargs", settings.kernel_cmdline) < 0) {
        printl("Failed to set cmdline!" NEWLINE);
        return 1;
    }

    //UNTESTED (but doesn't hurt)
    if(settings.initrd_loaded) {
        unsigned initrd_end = settings.initrd.addr + settings.initrd.size;
        if(fdt_setprop_cell(fdt, chosen, "linux,initrd-start", (unsigned)settings.initrd.addr) < 0
        || fdt_setprop_cell(fdt, chosen, "linux,initrd-end", initrd_end) < 0) {
            printl("Failed to set ramdisk location!" NEWLINE);
            return 1;
        }
    }

    fdt_pack(fdt);
    settings.boot_param.start = fdt;

    return 0;
}

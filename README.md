# TI-NSPIRE Linux In-Place Bootloader V2

In-place boot a Linux kernel from inside the Nspire operating system.

## Improvements over old one

This version of the bootloader has been rewritten to be more stable, portable and flexible than the previous version.

Some big improvements include:

 * Being able to load ramdisks and kernels larger than 4MB (up to ~40MB on the CX)
 * This version does not need to relocate anything by default which means much higher stability
 * Has a shell-like interface to set options for loading
 * Scripting support
 * Cleaner and more portable code
 * Easy to port to other models

## Usage

Copy ```linuxloader.tns``` to your calculator and run it.

Valid commands are:

 * ```kernel <filename>```: Loads a kernel image into memory
 * ```initrd <filename>```: Loads a ramdisk into memory
 * ```dump```: Prints out the current internal state of the bootloader. Useful for debugging.
 * ```free```: Prints out the total amount of memory provided to the bootloader by the Nspire OS and amounts used by the kernel and ramdisks.
 * ```cmdline [str]```: Get/set the kernel command line parameters.
 * ```mach [id]```: Get/set the machine ID that will be provided to Linux upon booting. Useful for overriding the builtin default value without having to recompile.
 * ```phys [<start> <size>]```: Get/set the address and size of physical memory. Useful for overriding the builtin default value without having to recompile.
 * ```rdsize [size]```: Get/set the size of the ramdisk that Linux should create on boot. Leave at ```0x0``` for the kernel default.
 * ```boot```: Boot kernel.

The bootloader is also scriptable. Create a text file containing a list of commands to be executed and change the extension to ```.ll2.tns```, and add the following line to your ```/documents/ndless/ndless.cfg.tns``` file.

```
ext.ll2=linuxloader2
```

Then simply open your script file and the loader will execute all the commands in it. A sample one could look like this:

```
kernel linux/zImage.tns
initrd linux/initrd.tns
cmdline root=/dev/ram
boot
```

This should save a lot of typing everytime you need to boot Linux.

## Developers

I've done my best to write nice, clean code but it is always open to improvements. Here are some notes for extending the bootloader to support more commands and models.

If you're having trouble compiling, try removing ```-Werror``` from the CFLAGS.

### Adding a new command

Commands are defined in ```cmd.c```. Scroll down until you see a list of ```DEFINE_COMMAND```s. Just follow the example to add your own.

### Adding a new bootloader variable

The bootloader's internal state is stored in a struct called ```settings```. It is defined in ```common.h```. Feel free to add your own bootloader variables but if you do, make sure you also update the ```dump_settings()``` function in ```common.c``` so it is dumped correctly.

### Porting to another calculator model

Most of the code is already portable to other calculators. The only parameters that need adjusting are the ones passed to Linux such as the amount of physical memory and machine ID.

At the moment, they are hardcoded values for the CX. Code to detect and automatically set the correct values for different models needs to be written. They can be manually overrided using ```mach``` and ```phys``` commands until proper detection is implemented.

If you're interested in writing an implementation for it, look in ```mach.c``` and implement the ```detect_memory()``` and ```detect_machine()``` functions.
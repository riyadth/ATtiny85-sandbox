# Sandbox for playing with ATtiny85 development

This repo contains a simple makefile and source files that I am using
to play with the ATtiny85 MCU APIs. The goals are to have code that
compiles, but does nothing useful (and may not actually run correctly),
and is a sort of "lab notebook" of AVR code snippets for use in my other
projects.

To add source files to the project, just place them in the src directory.
The makefile attempts to compile and link all files found in that directory.

## Dependencies

To compile this project you must install avr-gcc, and use avrdude to
flash on target hardware. I use Arch Linux, and installed the dependencies
via pacman.

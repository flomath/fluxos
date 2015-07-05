/*
 * console.h
 *
 * Created on: 02.07.15
 *     Author: Nino Schoch
 */

#ifndef SRC_CONSOLE_CONSOLE_H
#define SRC_CONSOLE_CONSOLE_H

#include "../system/system.h"

#define COMMAND_MAXLENGTH   64
#define COMMAND_MAXPARAMS   4

/**
 * Initialize console
 */
void console_init(void);

/**
 * Clear console window
 */
void console_clear(void);

/**
 * Process a given command
 * TODO: explanation
 */
void console_command(char* cmd, int argc, char** argv);

#endif //SRC_CONSOLE_CONSOLE_H

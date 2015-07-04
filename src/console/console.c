/*
 * console.c
 *
 * Created on: 02.07.15
 *     Author: Nino Schoch
 */

#include "console.h"
#include <string.h>

static void console_main(void);
static void print(char* text);
static void newLine();
static void println(char* text);
static char* console_read(uint32_t* length);
static void console_split_line(char* line, uint32_t length);

void console_init()
{
    console_main();
}

void console_clear()
{

}

void console_command(char* cmd, int argc, char** argv)
{
    //TODO: syscall for executing cmd?
    //TODO: free cmd and argv
}

static void console_main()
{
    uint32_t lineLength = 0;
    char* line = NULL;

    println("Welcome to FluxOS");

    while (1) {
    	print("root@flux:/# ");

        // read input
        line = console_read(&lineLength);

        // process read line
        console_split_line(line, lineLength);
        free(line);
    }
}

static void print(char* text)
{
    syscall(SYS_PRINT, (uint32_t*)text, strlen(text) + 1);
}

static void newLine()
{
    print("\n\r");
}

static void println(char* text)
{
    print(text);
    newLine();
}

static char* console_read(uint32_t* length)
{
    // create a line and init with 0
    char* line = malloc(COMMAND_MAXLENGTH * sizeof(char));
    memset(line, 0, COMMAND_MAXLENGTH);

    // read input
    int i;
    char c;
    int lineIndex = 0;
    for (i = 0; i < COMMAND_MAXLENGTH; i++) {
        //TODO: should whole input be read in syscall or really just each single char?
        syscall(SYS_READ, (uint32_t*)&c, 1);

        // if input char is \n break and process command
        if (c == '\n') {
            break;
        }
        //TODO: if input char == delete or other char which can "overwrite" other chars, handle it (arrow left...)
        // else if (...) {...}
        else {
            //TODO: check for valid character
            line[lineIndex] = c;
            lineIndex++;
        }
    }

    // set length, to have a clue, how long line is
    *length = lineIndex;
    return line;
}

static void console_split_line(char* line, uint32_t length)
{
    char** argv = malloc(COMMAND_MAXPARAMS * sizeof(char*));
    int argc = 0;
    char* cmd;

    int i;
    bool_t paramQuote = FALSE;
    int char_index = 0;
    char* char_container = malloc(COMMAND_MAXLENGTH * sizeof(char));
    for (i = 0; i < length; i++) {
        if (line[i] == '"') {
            paramQuote = !paramQuote;
        } else if (line[i] == ' ' && paramQuote == FALSE) {
            // if cmd is null, first add as cmd
            // afterwards
            if (cmd == NULL) {
                cmd = char_container;
            } else {
                argv[argc++] = char_container;
            }
            char_container = malloc(COMMAND_MAXLENGTH * sizeof(char));
        } else {
            char_container[char_index++] = line[i];
            char_container[char_index] = '\0'; //TODO: needed?
        }
    }

    console_command(cmd, argc, argv);
}

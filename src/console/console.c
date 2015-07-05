/*
 * console.c
 *
 * Created on: 02.07.15
 *     Author: Nino Schoch
 */

#include "console.h"
#include <string.h>
#include "../applications/audio/audio.h"

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

extern char appdata[];

void console_command(char* cmd, int argc, char* argv[])
{
    //TODO: syscall for executing cmd?
    //TODO: free cmd and argv
	if (strcmp(cmd, "echo") == 0) {
		println(argv[0]);
	}

	if (strcmp(cmd, "greetings") == 0) {
		uint32_t proc1[2] = {
			(uint32_t)&appdata,
			820
		};
		syscall(SYS_LOAD_PROC, proc1, 2); // Program Data + Main offset
	}

	if (strcmp(cmd, "playduck") == 0) {
		//syscall(SYS_START_PROC, (uint32_t*)0xE92D400E, 0);
		syscall(SYS_START_PROC, (uint32_t*)&audio_play_duck, 0);
	}

	int i;
	for ( i = 0; i < argc; i++) {
		*(argv[i]) = '\0';
		free(argv[i]);
	}
	*cmd = '\0';
	free(cmd);
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
        newLine();

        // There is at least one ' ' char
        if (lineLength > 1) {
            // process read line
            console_split_line(line, lineLength);
        }

        free(line);
    }
}

static void print(char* text)
{
    syscall(SYS_PRINT, (uint32_t*)text, strlen(text) + 1);
}

static void newLine()
{
    print("\r\n");
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
    char c;
    int lineIndex = 0;
    while (lineIndex < COMMAND_MAXLENGTH) {
        //TODO: should whole input be read in syscall or really just each single char?
        syscall(SYS_READ, (uint32_t*)&c, 1);

        if (c != '\0') {
        	// if input char is \n break and process command
			if (c == '\r' || c == '\n') {
				line[lineIndex] = ' ';
				lineIndex++;
				break;
			}
			//TODO: if input char == delete or other char which can "overwrite" other chars, handle it (arrow left...)
			// else if (...) {...}
			else if ((c >= 'a' && c <= 'z') || c == ' ') {
				//TODO: check for valid character
				line[lineIndex] = c;
				lineIndex++;
			}
        }
    }

    // set length, to have a clue, how long line is
    *length = lineIndex;
    return line;
}

static void console_split_line(char* line, uint32_t length)
{
    char* argv[COMMAND_MAXPARAMS];
    int argc = 0;
    char* cmd = NULL;
    char char_container[COMMAND_MAXLENGTH];

    int i;
    bool_t paramQuote = FALSE;
    int char_index = 0;
    for (i = 0; i < length; i++) {
        if (line[i] == '"') {
            paramQuote = !paramQuote;
        } else if (line[i] == ' ' && paramQuote == FALSE) {
            // if cmd is null, first add as cmd
            // afterwards
            if (cmd == NULL) {
            	cmd = (char*)malloc(COMMAND_MAXLENGTH * sizeof(char));
                strncpy(cmd, char_container, COMMAND_MAXLENGTH);
            } else {
            	char* argument = (char*)malloc(COMMAND_MAXLENGTH * sizeof(char));
            	strncpy(argument, char_container, COMMAND_MAXLENGTH);
            	argv[argc++] = argument;
            }
            char_index = 0;
        } else {
            char_container[char_index++] = line[i];
            char_container[char_index] = '\0'; //TODO: needed?
        }
    }

    console_command(cmd, argc, argv);
}

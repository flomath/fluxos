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

void console_init()
{


    console_main();
}

void console_clear()
{

}

void console_command(int argc, char** argv)
{

}

static void console_main()
{
    println("Das ist unser Text");

    while (1) {

    }
}

static void print(char* text)
{
    syscall(SYS_PRINT, (uint32_t*)text, strlen(text) + 1);
}

static void newLine()
{
    print("\n");
}

static void println(char* text)
{
    print(text);
    newLine();
}

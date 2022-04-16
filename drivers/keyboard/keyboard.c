#include "keyboard.h"
#include "../drivers/vga/display.h"
#include "../drivers/ports/ports.h"
#include "../cpu/interrupt.h"
#include "../cpu/global.h"
#include "../kernel/util.h"
#include "../kernel/kernel.h"
#define BACKSPACE 0x0E
#define ENTER 0x1C
static char key_buffer[256];

const char scan_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                           '7', '8', '9', '0', '-', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                           'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                           'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                           'B', 'N', 'M', ',', '.', '/', '?', '*', '?', ' '};

#define KBD_BUF_PORT 0x60

static void intr_keyboard_handler(void)
{
   uint8_t scancode = port_byte_in(KBD_BUF_PORT);
    if (scancode > 57) return;
    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (scancode == ENTER) {
        print_nl();
        execute_command(key_buffer);
        key_buffer[0] = '\0';
    } else {
        char letter = scan_ascii[(int) scancode];
        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print_string(str);
    }

   return;
}
void keyboard_init()
{
   register_handler(0x21, intr_keyboard_handler);
}

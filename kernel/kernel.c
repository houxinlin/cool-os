#include "../drivers/vga/display.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/ports/ports.h"
#include "util.h"
#include "../cpu/interrupt.h"
void main()
{

    // 清除屏幕
    clear_screen();
    idt_init();
    keyboard_init();
    asm volatile("sti");
}

//重启
void reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = port_byte_in(0x64);
    port_byte_out(0x64, 0xFE);
}
void print_int(int value)
{
    char result[255];
    itoa(value, result);
    print_string(result);
    print_nl();
}
void exec(int index, char *input, char ec_symbol)
{
    char temp[22];
    itoa(index, temp);
    int size = string_length(input);
    char ns_1[index];
    char ns_2[size - index];
    sub_string(input, 0, index, ns_1);
    sub_string(input, index + 1, -1, ns_2);
    print_string(input);
    print_string("=");
    if (ec_symbol == '+')
    {
        int value = atoi(ns_1) + atoi(ns_2);
        print_int(value);
    }
    if (ec_symbol == '-')
    {
        int value = atoi(ns_1) - atoi(ns_2);
        print_int(value);
    }
    if (ec_symbol == '*')
    {
        int value = atoi(ns_1) * atoi(ns_2);
        print_int(value);
    }
    if (ec_symbol == '/')
    {
        int value = atoi(ns_1) / atoi(ns_2);
        print_int(value);
    }
}
//回车会执行到这里
void execute_command(char *input)
{
    if(compare_string(input,"CLS")==0){
        clear_screen();
        return;
    }
    int index = -1;
    index = string_index_of(input, '-');
    if (index != -1)
        exec(index, input, '-');
    index = string_index_of(input, '+');
    if (index != -1)
        exec(index, input, '+');
    index = string_index_of(input, '*');
    if (index != -1)
        exec(index, input, '*');
    index = string_index_of(input, '/');
    if (index != -1)
        exec(index, input, '/');
}

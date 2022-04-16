#include "stdint.h"

#include "./interrupt.h"
#include "global.h"
#include "../drivers/vga/display.h"
#include "../drivers/ports/ports.h"

#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1
#define IDT_DESC_CNT 0x30
#define EFLAGS_IF 0x00000200
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" \
                                           : "=g"(EFLAG_VAR))

struct gate_desc
{
   uint16_t func_offset_low_word;
   uint16_t selector;
   uint8_t dcount;
   uint8_t attribute;
   uint16_t func_offset_high_word;
};

static void make_idt_desc(struct gate_desc *p_gdesc, uint8_t attr, intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT];

char *intr_name[IDT_DESC_CNT];

intr_handler idt_table[IDT_DESC_CNT];
extern intr_handler intr_entry_table[IDT_DESC_CNT];

static void pic_init(void)
{

   port_byte_out(PIC_M_CTRL, 0x11);
   port_byte_out(PIC_M_DATA, 0x20);
   port_byte_out(PIC_M_DATA, 0x04);
   port_byte_out(PIC_M_DATA, 0x01);

   port_byte_out(PIC_S_CTRL, 0x11);
   port_byte_out(PIC_S_DATA, 0x28);
   port_byte_out(PIC_S_DATA, 0x02);
   port_byte_out(PIC_S_DATA, 0x01);

   port_byte_out(PIC_M_DATA, 0xfd);
   port_byte_out(PIC_S_DATA, 0xff);
}

static void make_idt_desc(struct gate_desc *p_gdesc, uint8_t attr, intr_handler function)
{
   p_gdesc->func_offset_low_word = (uint32_t)function & 0x0000FFFF;
   p_gdesc->selector = SELECTOR_K_CODE;
   p_gdesc->dcount = 0;
   p_gdesc->attribute = attr;
   p_gdesc->func_offset_high_word = ((uint32_t)function & 0xFFFF0000) >> 16;
}

static void general_intr_handler(uint8_t vec_nr)
{
   if (vec_nr == 0x27 || vec_nr == 0x2f)
   {
      return;
   }
   while (1)
      ;
}

void register_handler(uint8_t vector_no, intr_handler function)
{
   idt_table[vector_no] = function;
}

void idt_init()
{
   int i;
   for (i = 0; i < IDT_DESC_CNT; i++)
   {
      make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);

      idt_table[i] = general_intr_handler;
      intr_name[i] = "unknown";
   }

   pic_init();
   uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt << 16));
   asm volatile("lidt %0"
                :
                : "m"(idt_operand));
}

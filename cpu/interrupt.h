#ifndef __KERNEL_INTERRUPT_H
#define __KERNEL_INTERRUPT_H
#include "stdint.h"
typedef void* intr_handler;
void idt_init(void);  //idt初始化，在进入内核时候调用
void register_handler(uint8_t vector_no, intr_handler function);//这是一个中断处理程序
#endif

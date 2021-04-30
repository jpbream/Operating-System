#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stdint.h>

#define DIVIDE_BY_ZERO 0x00
#define DEBUG_EXCEPTION 0x01
#define NON_MASKABLE_INTERRUPT 0x02
#define BREAKPOINT 0x03
#define OVERFLOW 0x04
#define BOUND_RANGE_EXCEEDED 0x05
#define INVALID_OPCODE 0x06
#define DEVICE_NOT_AVALIABLE 0x07
#define DOUBLE_FAULT 0x08
#define INVALID_TSS 0x0A
#define SEGMENT_NOT_PRESENT 0x0B
#define STACK_SEGMENT_FAULT 0x0C
#define GENERAL_PROTECTION_FAULT 0x0D
#define PAGE_FAULT 0x0E
#define FLOATING_POINT_EXCEPTION 0x10
#define ALIGNMENT_CHECK 0x11
#define MACHINE_CHECK 0x12
#define SIMD_FLOAT_EXCEPTION 0x13
#define VIRTUALIZATION_EXCEPTION 0x14
#define SECURITY_EXCEPTION 0x1E

const inline char* EXCEPTIONS[] =
{
    "Divide By Zero",
    "Debug Exception",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Reserved Exception",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved Exception",
    "Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Reserved Exception",
    "Security Exception",
    "Reserved Exception"
};

#define HAS_ERROR_CODE(error) (error == INVALID_TSS || error == SEGMENT_NOT_PRESENT || \
error == STACK_SEGMENT_FAULT || error == GENERAL_PROTECTION_FAULT)

#define IS_SELECTOR_EXTERNAL(error) (error & 0x1)
#define SELECTOR_SOURCE(error) ((error & 0x06) >> 1)
#define SELECTOR_INDEX(error) ((error & 0x0FFF8) >> 3)

enum SelectorSource {
    SELECTOR_SOURCE_GDT,
    SELECTOR_SOURCE_IDT,
    SELECTOR_SOURCE_LDT
};

#endif
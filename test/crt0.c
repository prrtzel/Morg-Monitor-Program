#include "stdint.h"
#include "crt0.h"


extern uint32_t end_stack;
extern uint32_t end_text;
extern uint32_t start_data;
extern uint32_t end_data;
extern uint32_t start_bss;
extern uint32_t end_bss;

__attribute__((section(".isr_vec")))
const uint32_t *isr_vectors[] = {
    (uint32_t*)&end_stack,                              // Initial Stack Pointer
    (uint32_t*)Reset_Handler,                         // Reset handler
    (uint32_t*)Bus_Error_Handler,                     // Bus Error handler
    (uint32_t*)Address_Error_Handler,                 // Address Error handler
    (uint32_t*)Illegal_Instruction_Handler,           // Illegal Instruction handler
    (uint32_t*)Zero_Divide_Handler,                   // Zero Divide handler
    (uint32_t*)CHK_Instruction_Handler,               // CHK Instruction handler
    (uint32_t*)TRAPV_Instruction_Handler,             // TRAPV Instruction handler
    (uint32_t*)Privilege_Violation_Handler,           // Privilege Violation handler
    (uint32_t*)Trace_Handler,                         // Trace handler
    (uint32_t*)Line_1010_Emulator_Handler,            // Line 1010 Emulator handler
    (uint32_t*)Line_1111_Emulator_Handler,            // Line 1111 Emulator handler
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    (uint32_t*)Level1_Interrupt_Auto_Vector_Handler,   // Level 1 Interrupt Auto_Vector handler
    (uint32_t*)Level2_Interrupt_Auto_Vector_Handler,   // Level 2 Interrupt Auto_Vector handler
    (uint32_t*)Level3_Interrupt_Auto_Vector_Handler,   // Level 3 Interrupt Auto_Vector handler
    (uint32_t*)Level4_Interrupt_Auto_Vector_Handler,   // Level 4 Interrupt Auto_Vector handler
    (uint32_t*)Level5_Interrupt_Auto_Vector_Handler,   // Level 5 Interrupt Auto_Vector handler
    (uint32_t*)Level6_Interrupt_Auto_Vector_Handler,   // Level 6 Interrupt Auto_Vector handler
    (uint32_t*)Level7_Interrupt_Auto_Vector_Handler,   // Level 7 Interrupt Auto_Vector handler
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
    0,                                              // Unassigned, Reserved
};
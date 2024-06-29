#pragma once
#ifndef CRT0_H
#define CRT0_H


//#define HW
#define SIM

void Reset_Handler(void);
void Bus_Error_Handler(void)__attribute__((weak));
void Address_Error_Handler(void)__attribute__((weak));
void Illegal_Instruction_Handler(void)__attribute__((weak));
void Zero_Divide_Handler(void)__attribute__((weak));
void CHK_Instruction_Handler(void)__attribute__((weak));
void TRAPV_Instruction_Handler(void)__attribute__((weak));
void Privilege_Violation_Handler(void)__attribute__((weak));
void Trace_Handler(void)__attribute__((weak));
void Line_1010_Emulator_Handler(void)__attribute__((weak));
void Line_1111_Emulator_Handler(void)__attribute__((weak));
void Level1_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));
void Level2_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));
void Level3_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));
void Level4_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));
void Level5_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));
void Level6_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));
void Level7_Interrupt_Auto_Vector_Handler(void)__attribute__((weak));


#endif

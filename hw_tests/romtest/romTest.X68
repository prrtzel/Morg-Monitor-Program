*----------------------------------------------------------- 

* Program    : Test Loop 

* Written by : Instructor 

* Date       : 2010 

* Description: Testing ROM. NOP is "No operation" and will 

*              simply increase the PC 

*----------------------------------------------------------- 

 

 
*-- asy68k file to assemble
 

SUPER_STACK     EQU     $00FF0  ;note this stack pointer points to a location in ROM and therefore will not function as a stack, it is simply used as a place holder. 

START   ORG     $000000 
        DC.L    SUPER_STACK 
        DC.L    MAIN
        
        ORG     $000200
MAIN    NOP
        NOP
        JMP     MAIN
        END     MAIN 








*~Font name~Courier New~
*~Font size~16~
*~Tab type~1~
*~Tab size~4~

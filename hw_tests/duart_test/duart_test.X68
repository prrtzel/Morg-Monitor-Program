DUART   EQU	    $00100000  loaded in A0 when needed, regs are offsets 
MR1A   	EQU   	1	Mode Register1 
MR2A  	EQU   	1	points here after MR1A is set 
SRA    	EQU   	3   Status Register (read) 
CSRA   	EQU   	3   Clock Select Register 
CRA   	EQU   	5	Command Register 
TBA		EQU   	7   Transfer Holding Register 
RBA		EQU   	7	Receive Holding Register 
ACR		EQU	  	9	Auxiliary control register 
IMR     EQU     11  Interrupt Mask Register

RxRDY	EQU		0	Recieve ready bit position 
TxRDY	EQU		2	Transmit ready bit position 
BAUD    EQU	    $CC baud rate value = 19,200 baud 
SUPER_STACK EQU $8FF00


    ORG $000000
    DC.L SUPER_STACK
    DC.L INIT
    
    ORG $000200
INIT:   
    NOP 
    JSR INIT_DUART
LOOP:
    JSR GET_CHAR
    JMP LOOP

INIT_DUART: 
    LEA	  	DUART,A0		A0 points to base DUART address 

; Software reset: 
    MOVE.B  #$50,CRA(A0)    Reset Break Change Interrupt
    MOVE.B  #$40,CRA(A0)    Reset Error Status
    MOVE.B	#$30,CRA(A0)	Reset TxA 
    MOVE.B	#$20,CRA(A0)	Reset RxA 
    MOVE.B	#$10,CRA(A0)	Reset MRA pointer 

; Initialization:
    MOVE.B  #$00,IMR(A0)    disables IMR
    MOVE.B  #$80,ACR(A0)	selects baud rate set 2
    MOVE.B  #BAUD,CSRA(A0) set 19.2k baud Rx/Tx 
    MOVE.B  #$13,MR1A(A0)   8-bits, no parity, 1 stop bit 

 
; This is the most important register to set in the 68681 DUART. 
; 07 sets: Normal mode, CTS and RTS disabled, stop bit length = 1 
; For testing load $#47 to enable auto-echo 

    MOVE.B	#$07,MR2A(A0)	 
    MOVE.B	#$05,CRA(A0)	enable Tx and Rx 
    RTS     
    
    
GET_CHAR:
    BTST    #2, SRA(A0)
    BEQ     GET_CHAR
    JSR     PUT_CHAR
    MOVE.B  RBA(A0), D1
    BRA     PUT_CHAR
    RTS


PUT_CHAR: 
    BTST    #2, SRA(A0)
    BEQ     PUT_CHAR
    MOVE.B  D1,TBA(A0) 
    RTS 
   
    
    END INIT

















*~Font name~Courier New~
*~Font size~16~
*~Tab type~1~
*~Tab size~4~

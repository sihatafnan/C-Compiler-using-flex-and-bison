.MODEL SMALL
.STACK 100H
.DATA
	i1 DW ?
	j1 DW ?
	k1 DW ?
	T1 DW ?
	T2 DW ?
	T3 DW ?
	T4 DW ?
	T5 DW ?
.CODE
;------printing procedure----
PRINT_ID PROC

	;SAVE IN STACK
	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX

	;CHECK IF NEGATIVE
	OR AX, AX
	JGE PRINT_NUMBER

	;PRINT MINUS SIGN
	PUSH AX
	MOV AH, 2
	MOV DL, '-'
	INT 21H
	POP AX

	NEG AX

	PRINT_NUMBER:
	XOR CX, CX
	MOV BX, 10D

	REPEAT_CALC:

		;AX:DX- QUOTIENT:REMAINDER
		XOR DX, DX
		DIV BX  ;DIVIDE BY 10
		PUSH DX ;PUSH THE REMAINDER IN STACK

		INC CX

		OR AX, AX
		JNZ REPEAT_CALC

	MOV AH, 2

	PRINT_LOOP:
		POP DX
		ADD DL, 30H
		INT 21H
		LOOP PRINT_LOOP

	;NEWLINE
	MOV AH, 2
	MOV DL, 0AH
	INT 21H
	MOV DL, 0DH
	INT 21H

	POP DX
	POP CX
	POP BX
	POP AX

	RET
PRINT_ID ENDP

MAIN PROC

	;INITIALIZE DATA SEGMENT
	MOV AX, @DATA
	MOV DS, AX

 
	MOV AX, 3
	MOV i1, AX
 
	MOV AX, 8
	MOV j1, AX
 
	MOV AX, 6
	MOV k1, AX
 
	MOV AX, i1
	CMP AX, 3
	JE Label1

	MOV T1, 0
	JMP Label2

	Label1:
	MOV T1, 1

	Label2:
;--------if block---------
	MOV AX, T1
	CMP AX, 0
	JE Label3
;--------print function called---------

	MOV AX, j1
	CALL PRINT_ID
	Label3:
 
	MOV AX, j1
	CMP AX, 8
	JL Label4

	MOV T2, 0
	JMP Label5

	Label4:
	MOV T2, 1

	Label5:
;--------if else block---------
	MOV AX, T2
	CMP AX, 0
	JE Label6
;--------print function called---------

	MOV AX, i1
	CALL PRINT_ID
	JMP Label7
	Label6:
;--------print function called---------

	MOV AX, k1
	CALL PRINT_ID

	Label7:
 
	MOV AX, k1
	CMP AX, 6
	JNE Label8

	MOV T3, 0
	JMP Label9

	Label8:
	MOV T3, 1

	Label9:
;--------if else block---------
	MOV AX, T3
	CMP AX, 0
	JE Label18
;--------print function called---------

	MOV AX, k1
	CALL PRINT_ID
	JMP Label19
	Label18:

	MOV AX, j1
	CMP AX, 8
	JG Label10

	MOV T4, 0
	JMP Label11

	Label10:
	MOV T4, 1

	Label11:
;--------if else block---------
	MOV AX, T4
	CMP AX, 0
	JE Label16
;--------print function called---------

	MOV AX, j1
	CALL PRINT_ID
	JMP Label17
	Label16:

	MOV AX, i1
	CMP AX, 5
	JL Label12

	MOV T5, 0
	JMP Label13

	Label12:
	MOV T5, 1

	Label13:
;--------if else block---------
	MOV AX, T5
	CMP AX, 0
	JE Label14
;--------print function called---------

	MOV AX, i1
	CALL PRINT_ID
	JMP Label15
	Label14:

	MOV AX, 0
	MOV k1, AX
 ;--------print function called---------

	CALL PRINT_ID

	Label15:

	Label17:

	Label19:
 
	MOV AX, 4CH
	INT 21H
MAIN ENDP

END MAIN
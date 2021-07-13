.MODEL SMALL
.STACK 100H
.DATA
	k1 DW ?
	T1 DW ?
	T2 DW ?
	T3 DW ?
	T4 DW ?
	x1 DW ?
	i1 DW ?
	T5 DW ?
	T6 DW ?
	T7 DW ?
	T8 DW ?
	T9 DW ?
	T10 DW ?
	T11 DW ?
	a1 DW ?
	b1 DW ?
	T12 DW ?
	T13 DW ?
	T14 DW ?
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

f PROC

	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX
 
	MOV AX, 5
	MOV k1, AX
 ;--------while loop---------
	Label3:

	MOV AX, k1
	CMP AX, 0
	JG Label1

	MOV T1, 0
	JMP Label2

	Label1:
	MOV T1, 1

	Label2:
	MOV AX, T1
	CMP AX, 0
	JE Label4
	MOV AX, a1
	INC AX
	MOV a1, AX
 	MOV AX, k1
	MOV T2, AX
	DEC AX
	MOV k1, AX
	JMP Label3
	Label4:
 
	MOV AX, 3
	MOV BX, a1
	MUL BX
	MOV T3, AX

	SUB AX, 7
	MOV T4, AX

	POP DX
	POP CX
	POP BX
	POP AX
RET
f ENDP

g PROC

	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX
 
	MOV AX, a1
	CALL f

	MOV AX, T4
	ADD AX, a1
	MOV T5, AX

	ADD AX, b1
	MOV T6, AX

	MOV x1, AX
 
	MOV AX, 0
	MOV i1, AX
;-------for loop starts--------
	Label11:

	MOV AX, i1
	CMP AX, 7
	JL Label5

	MOV T7, 0
	JMP Label6

	Label5:
	MOV T7, 1

	Label6:
	MOV AX, T7
	CMP AX, 0

	MOV AX, i1
	MOV BX, 3
	XOR DX, DX
	DIV BX
	MOV T8 , DX

	MOV AX, T8
	CMP AX, 0
	JE Label7

	MOV T9, 0
	JMP Label8

	Label7:
	MOV T9, 1

	Label8:
;--------if else block---------
	MOV AX, T9
	CMP AX, 0
	JE Label9

	MOV AX, x1
	ADD AX, 5
	MOV T10, AX

	MOV AX, T10
	MOV x1, AX
	Label9:

	MOV AX, x1
	SUB AX, 1
	MOV T11, AX
 	POP DX
	POP CX
	POP BX
	POP AX
RET
g ENDP

MAIN PROC

	;INITIALIZE DATA SEGMENT
	MOV AX, @DATA
	MOV DS, AX

 
	MOV AX, 1
	MOV a1, AX
 
	MOV AX, 2
	MOV b1, AX
 
	MOV AX, a1

	MOV AX, b1
	CALL g

	MOV AX, T11
	MOV a1, AX
 ;--------print function called---------

	CALL PRINT_ID
 
	MOV AX, 0
	MOV i1, AX
;-------for loop starts--------
	Label19:

	MOV AX, i1
	CMP AX, 4
	JL Label13

	MOV T12, 0
	JMP Label14

	Label13:
	MOV T12, 1

	Label14:
	MOV AX, T12
	CMP AX, 0
	JE Label20

	MOV AX, 3
	MOV a1, AX
 ;--------while loop---------
	Label17:

	MOV AX, a1
	CMP AX, 0
	JG Label15

	MOV T13, 0
	JMP Label16

	Label15:
	MOV T13, 1

	Label16:
	MOV AX, T13
	CMP AX, 0
	JE Label18
	MOV AX, b1
	INC AX
	MOV b1, AX
 	MOV AX, a1
	MOV T14, AX
	DEC AX
	MOV a1, AX
	JMP Label17
	Label18:
	MOV AX, i1
	INC AX
	MOV i1, AX
	JMP Label19
	Label20:
 ;--------print function called---------

	MOV AX, a1
	CALL PRINT_ID
 ;--------print function called---------

	MOV AX, b1
	CALL PRINT_ID
 ;--------print function called---------

	MOV AX, i1
	CALL PRINT_ID
 
	MOV AX, 4CH
	INT 21H
MAIN ENDP

END MAIN
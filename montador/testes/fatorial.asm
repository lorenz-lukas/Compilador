L1:      eQU 1;		1
L2: EQU 1;		2
l3: equ	3;		3
;			4
SECTION TEXT;		5
;l4: equ 0;		6
MUL_N: MACRO; 		7
MULT N;			8
;if l4;			9
STORE N;		10
END;			11
INPUT N;		12
LOAD N;			13
FAT: SUB ONE;qcbqq;	14
JMPZ FIM ;		15
STORE AUX;		16
MUL_N;			17
if 1 ;			18
LOAD AUX;		19
JMP FAT;		20
FIM: COPY N,N+1;	21
copy n+2,n+4;		22
IF L1;			23
OUTPUT N+1;		24
output L+1;		25
STOP;			26
;			27
SECTION DATA;		28
AUX: SPACE;		29
ONE: cONsT 1;		30
IF L2;			31
TWO: CONST 2;		32
if l3;			33
three: CONST 3;		34
;four: Const 4;		35
L: SPACE;		36
N: SPACE 5;		37
;L: SPACE ;		38

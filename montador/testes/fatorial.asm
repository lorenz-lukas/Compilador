L1:      eQU 1
L2: EQU 1
l3: equ	0

;l4: equ 0
TROCA: MACRO &A,&B,&TEMP
COPY &A, &TEMP
COPY &B, &A
COPY &TEMP, &B
mul_n
END
SECTION TEXT

MUL_N: MACRO			
mult n
STORE N;		
END;			
INPUT N;		
LOAD N;			
FAT: 



SUB ONE;qcbq
JMPZ FIM 
STORE AUX
MUL_N
troca n,j,l
if 1 ;
LOAD AUX
JMP FAT
FIM: COPY N,N+1
copy n+2,n+4
IF L1
OUTPUT N+1
output L+1
STOP

SECTION DATA
AUX: SPACE
ONE: cONsT 1
IF L2
TWO: CONST 2
if l3
three: CONST 3
;four: Const 4
L: SPACE
N: SPACE 5
J: SPACE 

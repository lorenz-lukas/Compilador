L1:      eQU 1
L2: EQU 1

section TEXT
IF L1
LOAD SPACE ;faz esta opera¸c~ao se L1 for verdadeiro
IF L2
INPUT SPACE ;faz esta opera¸c~ao se L2 for verdadeiro
MUL_N: MACRO
MULT N
STORE N
END

TROCA: MACRO &A,&B,&TEMP
  COPY A, TEMP
  MUL_N
  COPY B, A
  COPY TEMP, B
  END
lOaD N1
ROT:


INPUT N1
copy N1, N4 ; comentario qualquer
Troca N1,N2,N3
COPY N2,N3
COPY N3, N3 + 1
output N3 + 1
STOP

SECTION DATA ;a
N1: sPaCe ;jbajba
N2: CONST -0x10 ;		llkhg
N3: SPACE 2
N4: SPACE

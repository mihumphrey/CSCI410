@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@0
D=D+A
@13
M=D
@SP
AM=M-1
D=M
@13
A=M
M=D
(main$LOOP_START)
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
A=M-1
M=M+D
@LCL
D=M
@0	
D=D+A
@13
M=D
@SP
AM=M-1
D=M
@13
A=M
M=D
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
A=M-1
M=M-D
@ARG
D=M
@0
D=D+A
@13
M=D
@SP
AM=M-1
D=M
@13
A=M
M=D
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@main$LOOP_START
D;JNE
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
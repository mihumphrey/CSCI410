//pop temp 1
@SP
AM=M-1
D=M
@6
M=D
//pop temp 2
@SP
AM=M-1
D=M
@7
M=D
//push temp 1
@6
D=M
@SP
A=M
M=D
@SP
M=M+1
//push temp 2
@7
D=M
@SP
A=M
M=D
@SP
M=M+1

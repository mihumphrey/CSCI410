@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L0
D;JEQ
@SP
A=M-1
M=0
(L0)
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L1
D;JEQ
@SP
A=M-1
M=0
(L1)
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L2
D;JEQ
@SP
A=M-1
M=0
(L2)
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L3
D;JLT
@SP
A=M-1
M=0
(L3)
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L4
D;JLT
@SP
A=M-1
M=0
(L4)
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L5
D;JLT
@SP
A=M-1
M=0
(L5)
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L6
D;JGT
@SP
A=M-1
M=0
(L6)
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L7
D;JGT
@SP
A=M-1
M=0
(L7)
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
@SP
AM=M-1
D=M
@SP
A=M-1
D=M-D
M=-1
@L8
D;JGT
@SP
A=M-1
M=0
(L8)
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
@53
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
M=M+D
@112
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
@SP
A=M-1
M=-M
@SP
AM=M-1
D=M
@SP
A=M-1
M=M&D
@82
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
M=M|D
@SP
A=M-1
M=!M
(END)
@END
0;JMP

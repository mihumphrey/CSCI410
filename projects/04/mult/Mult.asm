// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.

<<<<<<< HEAD
/*
    ALGORITHM:
        R2 = 0
        while (R0 > 0)
            R2 += R1
            R0--
*/

@INIT
0;JMP

@0
D=A
@R0
M=D

@2
D=A
@R1
M=D

(INIT)
    @0
    D=A
    @R2
    M=D

(LOOP)
    @R0
=======
//
//    ALGORITHM:
//        R2 = 0
//        while (R0 > 0)
//            R2 += R1
//            R0--
//

// This block initializes R2 to zero so that the sum starts correctly
@0
D=A
@R2
M=D

// This is the main loop
//  Checks whether or not R0 is still positive
//    Ends if not
//  Increments R2=R2+R1
//  Decrements R0
(LOOP)
    @R0             
>>>>>>> 25689454fd6fd5f6557694e176e7c9a4c9f2ff58
    D=M
    @END
    D;JLE

<<<<<<< HEAD
    @R0
    M=M-1
=======
>>>>>>> 25689454fd6fd5f6557694e176e7c9a4c9f2ff58

    @R1
    D=M
    @R2
    M=M+D

<<<<<<< HEAD
    @LOOP
    0;JMP

=======
    @R0 
    M=M-1
    D=M

    @LOOP
    0;JMP

// This block ends the program
>>>>>>> 25689454fd6fd5f6557694e176e7c9a4c9f2ff58
(END)
    @END
    0;JMP

// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

@16383
D=A  
@idx
M=D  

(MAIN)
    @KBD 
    D=M 
    @WHITE 
    D;JEQ 

(BLACK)
    @idx 
    D=M+1 
    @KBD 
    D=A-D 
    @MAIN 
    D;JEQ  
    @idx  
    AM=M+1 
    M=!M  
    @MAIN 
    0;JMP

(WHITE)
    @16383  
    D=A  
    @idx  
    D=M-D  
    @MAIN 
    D;JEQ  
    @idx  
    A=M    
    M=!M  
    @idx  
    M=M-1  
    @MAIN 
    0;JMP 

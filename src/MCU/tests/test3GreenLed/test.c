org 0000h

SETB P0.6
d:LCALL delay
CPL P0.6
JMP d

delay:
    MOV R0,#250
    d1:MOV R1,#250
    d2:DJNZ R1,d2
    DJNZ R0,d1
RET

end

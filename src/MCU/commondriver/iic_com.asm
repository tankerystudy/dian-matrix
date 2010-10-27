;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; i2ccom.asm
;;;     the i2c protocol implementation interface file
;;; Usage(write):
;;;     1. Using '_ASMI2COpen' function with w mode to start the connection (#1010aax0b)
;;;     2. write a memory addr to EEPROM using '_ASMI2CWriteByte'
;;;     3. write what you want to store using '_ASMI2CWriteByte'
;;;     4. close connection using 'ASMI2CClose'
;;;
;;; Usage(Read):
;;;     1. Using '_ASMI2COpen' function with w mode to start the connection (#1010aax0b)
;;;     2. write a memory addr to EEPROM using '_ASMI2CWriteByte' (above 2 are optional)
;;;
;;;     1. Using '_ASMI2COpen' function with r mode to start the connection (#1010aax1b)
;;;     2. read what you want to store using '_ASMI2CWriteByte'
;;;     3. send NOACK
;;;     4. close connection using 'ASMI2CClose'
;;;

SCL equ P1.6
SDA equ P1.7

public _ASMI2COpen, _ASMI2CWriteByte, ASMI2CClose, ASMI2CReadByte, _ASMI2CSendACK, ASMI2CReset

asmi2ccodes segment code


    rseg asmi2ccodes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; OPEN the I2C device with specific name(device addr) & mode
;;;
;;;     input: R7, start byte, include name&mode
;;;     output: C, result of open, 1, succed, 0, failed.
;;;     local: A
;;;
_ASMI2COpen:
    call ASMI2CStart
    ; call write function to write open byte and return result
    call _ASMI2CWriteByte
ret

ASMI2CStart:
    clr  SCL
    setb SDA
    setb SCL
    clr  SDA
    clr  SCL        ; started, clear to ignore SDA
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; STOP and CLOSE the I2C device
;;;
;;;     input: none
;;;     output: none
;;;     local: none
;;;
ASMI2CClose:
    clr  SCL
    clr  SDA
    setb SCL
    setb SDA        ; set to free 2 line
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; WRITE a BYTE using register and return the acknowledge.
;;;
;;;     input:  R7, byte need write
;;;     output: C, result of writing, 1, succed, 0, failed.
;;;     local:  A
;;;
_ASMI2CWriteByte:
    push ACC        ; don't save psw, because the C need to return

    mov  A, R7
    mov  R7, #8     ; loop 8 time

WRITE_BIT:
    clr  SCL
    rlc  A
    mov  SDA, C     ; send bit from high
    setb SCL
    nop
    djnz R7, WRITE_BIT

    clr  SCL        ; clr clock line to allow changing
    setb SDA        ; set data line to AND with acknowledge
    setb SCL
    mov  C, SDA     ; read ackowledge from data line and return
    cpl  C          ; switch return value to match simple meaning.

    clr  SCL        ; started, clear to ignore SDA

    pop  ACC
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; READ a BYTE to register and return it.
;;;
;;;     output: R7, byte read to.
;;;     local:  A
;;;
ASMI2CReadByte:
    push PSW
    push ACC

    mov  R7, #8
    setb  SDA       ; set data line to allow changing on it.
READ_BIT:
    clr  SCL        ; for EEPROM to write SDA
    nop
    setb SCL
    mov  C, SDA     ; get bit
    rlc  A
    djnz R7, READ_BIT
    mov  R7, A      ; return it.

    clr  SCL        ; started, clear to ignore SDA

    pop  ACC
    pop  PSW
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Send a ACK or NOACK according to the parameter
;;;
;;;     input:  C, 1 to send ACK, else send NOACK
;;;     output: none
;;;     local:  A
;;;
_ASMI2CSendACK:
    push PSW
    push ACC

    mov  A, R7
    jz   SEND_NOACK ; if A==0, jmp to send NOACK
SEND_ACK:
    clr  SCL        ; send acknowledge
    clr  SDA
    setb SCL        ; sent
    nop
    clr  SCL
    setb SDA        ; reset SDA high
    jmp  ASMI2CSENDACK_END
SEND_NOACK:
    clr  SCL        ; send no acknowledge
    setb SDA
    setb SCL        ; sent
    nop
    clr  SCL        ; reset

ASMI2CSENDACK_END:
    pop  ACC
    pop  PSW
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; RESET the EEPROM device
;;;
;;;     input:  none
;;;     output: none
;;;     local:  R7
;;;
ASMI2CReset:
    call ASMI2CStart

    clr  SCL
    setb SDA
    mov  R7, #9
DUMMY_CLOCK_CYCLE:
    setb SCL
    nop
    clr  SCL
    djnz R7, DUMMY_CLOCK_CYCLE

    call ASMI2CStart
    call ASMI2CClose
ret



end

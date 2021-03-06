;; Basic msp430 ASM example

.include "msp430g2553.inc"

.org 0xf800

main:

  ;; disable watchdog timer
  ;; mov.w #(WDTPW | WDTHOLD), &WDTCTL
	
  mov.w #0xaffe, R10
  mov.w R10, R11
  mov.b R10, R11
  mov.w #0x1212, &0x0200
  mov.w #0x01fe, R5
  mov.w 2(R5), R4
  mov.w #0x0200, R6
  mov.w 2(R5), 2(R6)
  mov.w 2(R5), &0x0204 
  mov.w R10, 4(R6) 
  mov.w #0xabcd, 4(R6) 
  mov.w &0x0200, 6(R6) 

loop:
  jmp loop

;; .org 0x0122
;;  dw 0xbeef

.org 0xffe8

vectors:
  dw 0 
  dw 0
  dw 0
  dw 0
  dw 0
  dw 0  
  dw 0
  dw 0
  dw 0
  dw 0
  dw main
  dw main 

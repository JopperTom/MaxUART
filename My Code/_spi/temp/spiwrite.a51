NAME		spiwritebyte
;
;	spiwrite.a51	4-19-00	ott
;	This routine takes a byte variable and
;	shifts it out with the clock
;	worst case bit rate of 250kHz
;	byte rate of 35 kHz
;

?PR?SPIWRITEBYTE?MODULE	segment	code
?DT?SPIWRITEBYTE?MODULE	segment data overlayable

PUBLIC		_spiwritebyte, ?_spiwritebyte?BYTE

rseg	?DT?SPIWRITEBYTE?MODULE
?_spiwritebyte?BYTE:
d: ds 1

rseg	?PR?SPIWRITEBYTE?MODULE

CLKHIGH equ 00000010B	;Bitmask to turn clk pin high
CLKLOW equ 11111101B	;Bitmask to turn clk pin low
BITHIGH equ 00000100B	;Bitmask to turn out pin high
BITLOW equ 11111011B 	;Bitmask to turn out pin low

OUTC            XDATA        7F98H

_spiwritebyte:

	mov DPTR,#OUTC	;point to outc
	mov R6, #8	;set up loop
loop:	
	mov A, R7	;move data to send to A
	rlc A		;rotate left through carry
	mov R7, A	;save rotated
	movx A, @DPTR	;setup to change bit
	jc highbit	;if bit is high jump
	anl A, #BITLOW	;else set bit low
	sjmp skip	;skip setting bit high
highbit:
	orl A, #BITHIGH ;set out high and clock
skip:	
	orl A, #CLKHIGH	;set clock bit high
	movx @DPTR, A	;output data
;	nop		;may need this to stretch clock high time
	anl A, #CLKLOW	;set clock low
	movx @DPTR, A	;output low clock
	djnz R6, loop	;repeat eight times
	ret
	end


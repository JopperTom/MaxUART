NAME		spireadbyte
;
;	spiread.a51	4-19-00	ott
;	This routine shifts in a byte
;	with the clock and returns it
;	worst case bit rate of 222kHz
;	byte rate of 29 kHz
;

?PR?SPIREADBYTE?MODULE	segment	code

PUBLIC		spireadbyte

CLKHIGH equ 00000010B	;Bitmask to turn clk pin high
CLKLOW equ 11111101B		;Bitmask to turn clk pin low
BITMASK equ 00001000B	;Bitmask to read input pin

OUTC            XDATA        7F98H
PINSC		XDATA		7F9BH
DPS		DATA		86H

rseg	?PR?SPIREADBYTE?MODULE
spireadbyte:

	mov DPTR,#OUTC	;point to outc
	movx A,@DPTR	;read outc
	mov R4,A	;save outc
	orl A,#CLKHIGH	;set clock high byte
	mov R5,A	;save contents of clkhigh
	mov A,R4	;read outc
	anl A,#CLKLOW	;set clock low byte
	mov R4,A	;save contents of clklow
	inc DPS		;switch pointer 1
	mov DPTR,#PINSC	;point second pointer to pinsc
	inc DPS		;switch pointer 0
	mov R6,#8	;set up loop
loop:	
	mov A, R5	;get contents of clkhigh
	movx @DPTR, A	;output data
	inc DPS		;switch pointer 1
	movx A,@DPTR	;read in port c
	anl A,#BITMASK	;mask off input pin
	jnz highbit	;if result is not zero
	clr c		;clear carry
	sjmp skip	;skip carry set
highbit:
	setb c		;set carry
skip:	
	inc DPS		;switch pointer 0
	mov A,R4	;get contents of clklow
	movx @DPTR,A	;output low clock
	mov A,R7	;move byte into A
	rlc A		;rotate bit in
	mov R7,A	;save new byte
	djnz R6,loop	;repeat eight times
	ret
	end



@echo off
REM #--------------------------------------------------------------------------
REM #	File:		BUILD.BAT
REM #	Contents:	Batch file to build development board i/o example firmware.
REM #
REM #	Copyright (c) 1997 AnchorChips, Inc. All rights reserved
REM #--------------------------------------------------------------------------

REM ## Compile C source file ##
c51 dev_io.c debug objectextend small moddp2

REM ## Link binary object with debug info ##
echo dev_io.obj, %CYUSB%\Target\Lib\Ezusb\EZUSB.lib  > tmp.rsp
echo TO dev_io RAMSIZE(256)  PL(68) PW(78) CODE(4000h) XDATA(5000h)  >> tmp.rsp
bl51 @tmp.rsp

REM ## Generate intel hex image without debug info ##
oh51 dev_io HEXFILE(dev_io.hex)

REM ### Generate BIX image of binary (no debug info) ###
%CYUSB%\Bin\hex2bix dev_io.hex

REM ### usage: build -clean to remove intermediate files after build
if "%1" == "-clean" del *.lst
if "%1" == "-clean" del *.obj
if "%1" == "-clean" del *.m51

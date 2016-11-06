@echo off
set mydir=%~dp0
Powershell -Command "& { Start-Process \"%mydir%tcc_gui.exe\" -verb RunAs}"
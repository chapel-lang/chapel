rem
rem $Id: make.bat,v 1.15 1999/03/16 18:34:52 pvmsrc Exp $
rem
rem echo on
@echo off
cls
rem Script File for PVM for WIN32
rem 06/24/1997  Markus Fischer
rem 10/--/1998	Stephen L. Scott -- revised edition
rem 01/16/1999	Stephen L. Scott -- resolve some NT, Win95/98 issues
rem 02/17/1999	Stephen L. Scott -- DevStudio C/Fortran V6 vs V5 differences

rem
rem OS is used to decide if batch file has to do any work
rem to initialize C or Fortran setup.  NT does not require
rem any setup while 95/98 has some interesting twists...
rem
if "%OS%" == "Windows_NT" goto BEGIN

rem skip 95/98 tests if have already been through once
if "%PVMVCF%" == "YES" goto BEGIN

rem ***********
rem * FORTRAN *
rem ***********
rem check if dfvars.bat has been executed
rem    DF_TOOLS is defined as MSVC in dfvars.bat, so test for it...
rem    if found - dfvars.bat already initialized VC
rem               so skip VC stuff below too
if "%DF_TOOLS%" == "MSVC" goto BEGIN

rem dfvars.bat not executed, determine if fortran is installed
rem   decide which based on explicit path to fortran location.
rem   if find here, then execute dfvars.bat and skip C stuff below.
if exist "%PVM_FBIN%\dfvars.bat" goto HASFORTRAN
echo.
echo Fortran compiler not found.
echo.

rem ******
rem * VC *
rem ******
rem check if vcvars32.bat has been executed
rem    MSVCDIR is defined in vcvars.bat, so test for it...
rem    if found - vcvars32.bat already initialized VC
rem               so skip VC stuff below
if exist "%MSVCDIR%\bin\vcvars32.bat" goto BEGIN

rem either vcvars32.bat not executed OR VC not installed
rem   decide which based on explicit path to VC location.
rem if find here, then execute vcvars32.bat
if exist "%PVM_CBIN%\vcvars32.bat" goto HASC
echo.
echo C compiler not found.
echo.
goto INTRO

:HASFORTRAN
CALL %PVM_FBIN%\dfvars.bat
rem this does VC also - so skip VC stuff...
goto BEGIN

:HASC
CALL %PVM_CBIN%\vcvars32.bat
goto BEGIN


:BEGIN
set PVMVCF=YES
if "%1" == "" goto INTRO
goto SWITCH

:INTRO
echo This is a script for (re)compiling the PVM code and examples.
echo Type one of the following at the DOS command line.
echo.
echo make pvm     -- will compile and install the PVM code by
echo                 compiling libpvm, daemon, hoster, groupserver
echo                 and console
echo. 
echo make ec      -- will compile the C examples
echo.
echo make ef      -- will compile the Fortran examples
echo.
echo make gec     -- will compile the C group examples
echo.
echo make gef     -- will compile the Fortran group examples
echo.
echo make all     -- will compile the items above
echo.
echo make clean   -- will erase all object files (.obj)
echo.
echo make tidy    -- will erase the object, dll, and executables
echo                 (.obj .dll .ilk .pdb .exe)
echo.
echo make tec     -- perform a "tidy" for C examples
echo.
echo make tef     -- perform a "tidy" for Fortran examples
echo.
echo make tgec    -- perform a "tidy" for C group examples
echo.
echo make tgef    -- perform a "tidy" for Fortran group examples
echo.

echo.
goto ENDE

:SWITCH
if "%1" == "pvm" goto PVM
if "%1" == "all" goto PVMEX 
if "%1" == "clean" goto CLEAN
if "%1" == "tidy" goto TIDY
if "%1" == "ec" goto CEXAMPLES
if "%1" == "ef" goto FEXAMPLES
if "%1" == "gec" goto CGEXAMPLES
if "%1" == "gef" goto FGEXAMPLES
if "%1" == "tec" goto TEC
if "%1" == "tef" goto TEF
if "%1" == "tgec" goto TGEC
if "%1" == "tgef" goto TGEF

echo Your make "option" specification did not match. RETRY...
echo.
goto INTRO

:PVM
:PVMEX
echo.
echo Compiling and installing library (C,Fortran), daemon, tracer,
echo console, hoster and groupserver
echo -------------------------------------------------------------
cd %PVM_ROOT%\src\
nmake /f makefile.mak
cd %PVM_ROOT%
echo.
echo Compiling and installing tracer
echo -------------------------------
cd %PVM_ROOT%\tracer\
nmake /f makefile.mak
cd %PVM_ROOT%
echo.
echo Compiling and installing pvm
echo ----------------------------
cd %PVM_ROOT%\console\
nmake /f makefile.mak
cd %PVM_ROOT%
echo.
echo Compiling and installing hoster
echo -------------------------------
cd %PVM_ROOT%\hoster\
nmake /f makefile.mak
cd %PVM_ROOT%
echo.
echo Compiling and installing pvmgs
echo ------------------------------
cd %PVM_ROOT%\pvmgs\
nmake /f makefile.mak
cd %PVM_ROOT%
echo.
echo Compiling and installing libfpvm
echo --------------------------------
cd %PVM_ROOT%\libfpvm\
nmake /f makefile.mak
cd %PVM_ROOT%
echo.
echo More Information in Readme.win32
echo.
if "%1" == "pvm" goto ENDE

:CEXAMPLES
cd %PVM_ROOT%\examples
echo.
echo Building C examples
echo -------------------
nmake /f makefile.mak c-all
cd %PVM_ROOT%
if "%1" == "ec" goto ENDE

:FEXAMPLES
echo.
echo Building Fortran examples
echo -------------------------
cd %PVM_ROOT%\examples
nmake /f makefile.mak f-all
cd %PVM_ROOT%
if "%1" == "ef" goto ENDE

:CGEXAMPLES
cd %PVM_ROOT%\gexamples
echo.
echo Building C group examples
echo -------------------------
nmake /f makefile.mak c-all
cd %PVM_ROOT%
if "%1" == "gec" goto ENDE

:FGEXAMPLES
echo.
echo Building Fortran group examples
echo -------------------------------
rem skipout here if doing "all" as there is a known
rem problem with the fortran group example code
if "%1" == "all" goto ENDE
rem feel free to let it fail if you really want to try it...
cd %PVM_ROOT%\gexamples
nmake /f makefile.mak f-all
cd %PVM_ROOT%
if "%1" == "gef" goto ENDE

goto ENDE

:TIDY
echo.
echo Cleaning Execs and Libs ...
echo ---------------------------
cd %PVM_ROOT%\bin\%PVM_ARCH%\
erase *.exe
cd %PVM_ROOT%\tracer\%PVM_ARCH%\
erase *.exe
erase *.lib
cd %PVM_ROOT%\lib\%PVM_ARCH%\
erase *.lib
erase *.exe
erase *.dll
cd %PVM_ROOT%\libfpvm\%PVM_ARCH%\
erase *.lib
erase *.dll
cd %PVM_ROOT%

:CLEAN
echo.
echo Cleaning Objects ...
echo --------------------
cd %PVM_ROOT%\src\%PVM_ARCH%\
erase *.obj
echo.
echo Cleaning libfpvm objects ...
echo ----------------------------
cd %PVM_ROOT%\libfpvm\%PVM_ARCH%
erase *.obj
echo.
echo Cleaning console objects ...
echo ----------------------------
cd %PVM_ROOT%\console\%PVM_ARCH%
erase *.obj
echo.
echo Cleaning hoster objects ...
echo ---------------------------
cd %PVM_ROOT%\hoster\%PVM_ARCH%\
erase *.obj
echo.
echo Cleaning pvmgs objects ...
echo --------------------------
cd %PVM_ROOT%\pvmgs\%PVM_ARCH%\
erase *.obj
echo.
echo Cleaning tracer objects ...
echo ---------------------------
cd %PVM_ROOT%\tracer\%PVM_ARCH%\
erase *.obj
echo.
echo Cleaning example objects ...
echo ----------------------------
cd %PVM_ROOT%\examples\%PVM_ARCH%\
erase *.obj
cd %PVM_ROOT%\libfpvm\%PVM_ARCH%\
erase *.ilk
erase *.pdb
echo.
echo Cleaning group example objects ...
echo ----------------------------------
cd %PVM_ROOT%\gexamples\%PVM_ARCH%\
erase *.obj

cd %PVM_ROOT%
goto ENDE

:TEC
echo.
echo Tidy the C examples
echo -------------------
cd %PVM_ROOT%\examples
nmake /f makefile.mak tidy-c
cd %PVM_ROOT%
goto ENDE

:TEF
echo.
echo Tidy the Fortran examples
echo -------------------------
cd %PVM_ROOT%\examples
nmake /f makefile.mak tidy-f
cd %PVM_ROOT%
goto ENDE

:TGEC
echo.
echo Tidy the C group examples
echo -------------------------
cd %PVM_ROOT%\gexamples
nmake /f makefile.mak tidy-c
cd %PVM_ROOT%
goto ENDE

:TGEF
echo.
echo Tidy the Fortran group examples
echo -------------------------
cd %PVM_ROOT%\gexamples
nmake /f makefile.mak tidy-f
cd %PVM_ROOT%
goto ENDE


:ENDE

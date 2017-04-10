@echo off
echo INFORMATION:
echo.
if "%QTDIR%"=="" goto noqt
if not exist %QTDIR%\bin goto noqt
echo Qt is in: %QTDIR%
echo.
echo         Menu
echo.
echo  1. Run Qmake to generate MSVC project
echo  2. Run Qmake to include new files
echo  3. Delete win32 directory content
echo  4. Delete temporal/backup files
echo.
echo  Press any key to exit.
echo.
set /p OPTION=
if "%OPTION%"=="4" goto option4
if "%OPTION%"=="3" goto option3
if "%OPTION%"=="2" goto option2
if "%OPTION%"=="1" goto option1
goto end

:option1
echo Are you developing an application in the robot computer?
echo "[y]es / [N]o :"
set /p ROBI=
if "%ROBI%"=="Y" goto robi
if "%ROBI%"=="y" goto robi
qmake -spec win32-msvc2008 projekt\main.pro -o win32\projekt.vcproj
if not errorlevel 0 pause
goto end

:robi
qmake -spec %QTDIR%\mkspecs\win32-msvc -cache %CD%\projekt\qmake.cache projekt\main.pro -o win32\projekt.dsp
if not errorlevel 0 pause
goto end

:option2
qmake -project -o projekt\sources.pri
if not errorlevel 0 pause
goto end

:option3
rem Delete win32 content
attrib -h -r +a win32\*.*
del win32\*.*
del win32\tmp\*.*
del win32\bin\*.*
goto end

:option4
rem Delete temporal/backup files
del src\*.h~
del src\*.cpp~
del *.*~
goto end

:noqt
echo Please define the enviroment variable QTDIR
echo with the value of Qt library directory
echo.
pause
goto end

:end


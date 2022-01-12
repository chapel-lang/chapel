set cmdline=%1
set out_file=%2
set result_file=%3


for /f "tokens=*" %%n in (%cmdline%) do set test_cmdline=%%n
%test_cmdline% >%out_file% 2>&1
echo %errorlevel% >%result_file%.tmp
ren %result_file%.tmp %result_file%

exit

@echo off
setlocal EnableDelayedExpansion

set PATH=%~dp0;%PATH%


set S_INTERFACE=127.0.0.1
set C_INTERFACE=127.0.0.1

set TEST_FAIL_TIMEOUT=90

set unit_tests=^
	"av_test -g 192.168.10.1 -n 1 -s 127.0.0.1"^
	"eq_test"
rem Disabling this test since it fails on windows (appveyor). Re-enable after root cause is identified and fixed
rem "dom_test -n 2"

set functional_tests=^
	"cq_data"^
	"dgram -p sockets"^
	"dgram_waitset -p sockets"^
	"msg"^
	"msg_sockets"^
	"poll -t queue"^
	"poll -t counter"^
	"rdm"^
	"rdm_rma_event"^
	"rdm_rma_trigger"^
	"rdm_tagged_peek"^
	"bw -e rdm -v -T 1"^
	"bw -e msg -v -T 1"^
	"scalable_ep"
rem	"msg_epoll"

set short_tests=^
	"msg_pingpong -I 5"^
	"msg_pingpong -I 5 -v"^
	"rdm_cntr_pingpong -I 5"^
	"rdm_pingpong -I 5"^
	"rdm_pingpong -I 5 -v"^
	"rdm_tagged_pingpong -I 5"^
	"rdm_tagged_pingpong -I 5 -v"
rem	"msg_bw -I 5"^
rem	"msg_bw -I 5 -v"^
rem	"rma_bw -e msg -o write -I 5"^
rem	"rma_bw -e msg -o read -I 5"^
rem	"rma_bw -e msg -o writedata -I 5"^
rem	"rma_bw -e rdm -o write -I 5"^
rem	"rma_bw -e rdm -o read -I 5"^
rem	"rma_bw -e rdm -o writedata -I 5"^
rem	"msg_rma -o write -I 5"^
rem	"msg_rma -o read -I 5"^
rem	"msg_rma -o writedata -I 5"^
rem	"msg_stream -I 5"^
rem	"rdm_atomic -I 5 -o all"^
rem	"rdm_multi_recv -I 5"^
rem	"rdm_rma -o write -I 5"^
rem	"rdm_rma -o read -I 5"^
rem	"rdm_rma -o writedata -I 5"^
rem	"rdm_tagged_bw -I 5"^
rem	"rdm_tagged_bw -I 5 -v"^
rem	"dgram_pingpong -I 5"^


set standard_tests=^
	"msg_pingpong"^
	"msg_pingpong -v"^
	"msg_pingpong -k"^
	"msg_pingpong -k -v"^
	"rdm_cntr_pingpong"^
	"rdm_pingpong"^
	"rdm_pingpong -v"^
	"rdm_pingpong -k"^
	"rdm_pingpong -k -v"^
	"rdm_tagged_pingpong"^
	"rdm_tagged_pingpong -v"
rem	"msg_bw"^
rem	"msg_bw -v"^
rem	"rma_bw -e msg -o write"^
rem	"rma_bw -e msg -o read"^
rem	"rma_bw -e msg -o writedata"^
rem	"rma_bw -e rdm -o write"^
rem	"rma_bw -e rdm -o read"^
rem	"rma_bw -e rdm -o writedata"^
rem	"msg_rma -o write"^
rem	"msg_rma -o read"^
rem	"msg_rma -o writedata"^
rem	"msg_stream"^
rem	"rdm_atomic -o all -I 1000"^
rem	"rdm_multi_recv"^
rem	"rdm_rma -o write"^
rem	"rdm_rma -o read"^
rem	"rdm_rma -o writedata"^
rem	"rdm_tagged_bw"^
rem	"rdm_tagged_bw -v"^
rem	"dgram_pingpong"^
rem	"dgram_pingpong -k"^


set test_types=unit functional short


goto :main


:single__clear_txt
	del out.txt res.txt >nul 2>nul
exit /b 0
:single__start_job
	start /b run_with_output.cmd "!test_cmdline!" out.txt res.txt >nul 2>nul
exit /b 0
:single__check_is_job_ended
	if not exist res.txt exit /b 1
exit /b 0
:single__print_output
	echo OUTPUT: & type out.txt & echo ----
exit /b 0
:single__print_and_check_err_code
	setlocal
	set err=0
	set /p err_code= <res.txt || ( echo error getting error code >&2 & exit /b 2 )
	set /a err_code=err_code
	if not "!err_code!"=="0" ( set err=1 & echo error code = !err_code! )
exit /b !err!


:client_server__clear_txt
	del out_sv.txt res_sv.txt out_cl.txt res_cl.txt >nul 2>nul
exit /b 0
:client_server__start_job
	start /b run_with_output.cmd "!test_cmdline! -s !S_INTERFACE!" out_sv.txt res_sv.txt >nul 2>nul
	start /b run_with_output.cmd "!test_cmdline! -s !C_INTERFACE! !S_INTERFACE!" out_cl.txt res_cl.txt >nul 2>nul
exit /b 0
:client_server__check_is_job_ended
	if not exist res_sv.txt exit /b 1
	if not exist res_cl.txt exit /b 1
exit /b 0
:client_server__print_output
	echo SERVER OUTPUT: & type out_sv.txt & echo ----
	echo CLIENT OUTPUT: & type out_cl.txt & echo ----
exit /b 0
:client_server__print_and_check_err_code
	setlocal
	set err=0
	set /p sv_err_code= <res_sv.txt || ( echo error getting server error code >&2 & exit /b 2 )
	set /p cl_err_code= <res_cl.txt || ( echo error getting client error code >&2 & exit /b 2 )
	set /a sv_err_code=sv_err_code
	set /a cl_err_code=cl_err_code
	if not "!sv_err_code!"=="0" ( set err=1 & echo server error code = !sv_err_code! )
	if not "!cl_err_code!"=="0" ( set err=1 & echo client error code = !cl_err_code! )
exit /b !err!


:results
	setlocal
	set err=%1
	call :!test_type!__print_output
	if "!err!"=="2" (
		echo test failed due to timeout
	) else (
		if not "!err!"=="0" (
			echo UNKNOWN ERROR
			exit -1
		) else (
			call :!test_type!__print_and_check_err_code
			set err=!errorlevel!
			if not "!err!"=="0" echo test failed
		)
	)
exit /b !err!




:run
	setlocal
	for /f "tokens=1" %%n in ("!test_cmdline!") do set test_exename=%%n.exe

	call :!test_type!__start_job
	set /a secs=0
	:run_client_server__WAIT
		set /a secs=secs+1
		timeout /t 1 >nul
		if /I !secs! geq %TEST_FAIL_TIMEOUT% (
			echo timeout, killing job
			taskkill /f /im !test_exename! >nul
			exit /b 2
		)
		call :!test_type!__check_is_job_ended || goto :run_client_server__WAIT
	:run_client_server__WAIT_END
exit /b 0





:test
	setlocal
	set test_type=%1
	for /f "tokens=*" %%n in (%2) do set test_cmdline=%%n
	echo !test_cmdline!

	call :!test_type!__clear_txt
	call :run
	call :results !errorlevel!
	set err=!errorlevel!
	call :!test_type!__clear_txt
exit /b !err!




:run_test__unit
	call :test single %*
exit /b


:run_test__functional
	call :test client_server %*
exit /b


:run_test__standard
	call :test client_server %*
exit /b


:run_test__short
	call :test client_server %*
exit /b




:main
	set /a tests_count=0
	set /a failed_tests_count=0
	for %%a in (!test_types!) do (
		for %%t in (!%%a_tests!) do (
			set /a tests_count=tests_count+1
		)
	)

	for %%a in (!test_types!) do (
		echo %%a tests:
		set run=run_test__%%a
		for %%t in (!%%a_tests!) do (
			call :!run! %%t || (
				set failed=!failed! %%t
				set /a failed_tests_count=failed_tests_count+1
			)
		)
		echo.
		echo ==============================
	)


	if not "!failed!"=="" (
		echo !failed_tests_count! of !tests_count! tests failed:
		for %%t in (!failed!) do (
			echo %%t
		)
		exit /b !failed_tests_count!
	)

	echo all !tests_count! tests passed


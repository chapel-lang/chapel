#!/usr/bin/env bash

#
# Copyright (c) 2017-2019, Intel Corporation.  All rights reserved.
# Copyright (c) 2016-2018, Cisco Systems, Inc. All rights reserved.
# Copyright (c) 2016, Cray, Inc. All rights reserved.
# Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
#
# This software is available to you under a choice of one of two
# licenses.  You may choose to be licensed under the terms of the GNU
# General Public License (GPL) Version 2, available from the file
# COPYING in the main directory of this source tree, or the
# BSD license below:
#
#     Redistribution and use in source and binary forms, with or
#     without modification, are permitted provided that the following
#     conditions are met:
#
#      - Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#
#      - Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials
#        provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

trap cleanup_and_exit SIGINT

#
# Default behavior with no args will use sockets provider with loopback
#
declare BIN_PATH
declare PROV=""
declare CORE=""
declare UTIL=""
declare TEST_TYPE="quick"
declare SERVER="127.0.0.1"
declare CLIENT="127.0.0.1"
declare GOOD_ADDR=""
declare -i VERBOSE=0
declare -i SKIP_NEG=0
declare COMPLEX_CFG
declare TIMEOUT_VAL="120"
declare STRICT_MODE=0
declare FORK=0
declare OOB=0
declare C_ARGS=""
declare S_ARGS=""

declare cur_excludes=""
declare file_excludes=""
declare input_excludes=""

declare -r c_outp=$(mktemp fabtests.c_outp.XXXXXX)
declare -r s_outp=$(mktemp fabtests.s_outp.XXXXXX)

declare -i skip_count=0
declare -i pass_count=0
declare -i fail_count=0
declare -i total_failures=0

python=$(which python3 2>/dev/null) || python=$(which python2 2>/dev/null) || python=$(which python 2>/dev/null)

if [ $? -ne 0 ]; then
	echo "Unable to find python dependency, exiting..."
	exit 1
fi

if [[ "$(uname)" == "FreeBSD" ]]; then
    declare -ri FI_ENODATA=$($python -c 'import errno; print(errno.ENOMSG)')
else
    declare -ri FI_ENODATA=$($python -c 'import errno; print(errno.ENODATA)')
fi
declare -ri FI_ENOSYS=$($python -c 'import errno; print(errno.ENOSYS)')

neg_unit_tests=(
	"fi_dgram g00n13s"
	"fi_rdm g00n13s"
	"fi_msg g00n13s"
)

functional_tests=(
	"fi_av_xfer -e rdm"
	"fi_av_xfer -e dgram"
	"fi_cm_data"
	"fi_cq_data -e msg"
	"fi_cq_data -e rdm"
	"fi_cq_data -e dgram"
	"fi_dgram"
	"fi_dgram_waitset"
	"fi_msg"
	"fi_msg_epoll"
	"fi_msg_sockets"
	"fi_poll -t queue"
	"fi_poll -t counter"
	"fi_rdm"
	"fi_rdm -U"
	"fi_rdm_rma_event"
	"fi_rdm_rma_trigger"
	"fi_shared_ctx"
	"fi_shared_ctx --no-tx-shared-ctx"
	"fi_shared_ctx --no-rx-shared-ctx"
	"fi_shared_ctx -e msg"
	"fi_shared_ctx -e msg --no-tx-shared-ctx"
	"fi_shared_ctx -e msg --no-rx-shared-ctx"
	"fi_shared_ctx -e dgram"
	"fi_shared_ctx -e dgram --no-tx-shared-ctx"
	"fi_shared_ctx -e dgram --no-rx-shared-ctx"
	"fi_rdm_tagged_peek"
	"fi_scalable_ep"
	"fi_rdm_shared_av"
	"fi_multi_mr -e msg -V"
	"fi_multi_mr -e rdm -V"
	"fi_recv_cancel -e rdm -V"
	"fi_unexpected_msg -e msg -i 10"
	"fi_unexpected_msg -e rdm -i 10"
	"fi_inj_complete -e msg"
	"fi_inj_complete -e rdm"
	"fi_inj_complete -e dgram"
	"fi_inj_complete -e msg -SR"
	"fi_inj_complete -e rdm -SR"
	"fi_inj_complete -e dgram -SR"
	"fi_bw -e rdm -v -T 1"
	"fi_bw -e rdm -v -T 1 -U"
	"fi_bw -e msg -v -T 1"
)

short_tests=(
	"fi_msg_pingpong -I 5"
	"fi_msg_pingpong -I 5 -v"
	"fi_msg_bw -I 5"
	"fi_msg_bw -I 5 -v"
	"fi_rma_bw -e msg -o write -I 5"
	"fi_rma_bw -e msg -o read -I 5"
	"fi_rma_bw -e msg -o writedata -I 5"
	"fi_rma_bw -e rdm -o write -I 5"
	"fi_rma_bw -e rdm -o write -I 5 -U"
	"fi_rma_bw -e rdm -o read -I 5"
	"fi_rma_bw -e rdm -o read -I 5 -U"
	"fi_rma_bw -e rdm -o writedata -I 5"
	"fi_rma_bw -e rdm -o writedata -I 5 -U"
	"fi_rdm_atomic -I 5 -o all"
	"fi_rdm_atomic -I 5 -o all -U"
	"fi_rdm_cntr_pingpong -I 5"
	"fi_multi_recv -e rdm -I 5"
	"fi_multi_recv -e msg -I 5"
	"fi_rdm_pingpong -I 5"
	"fi_rdm_pingpong -I 5 -U"
	"fi_rdm_pingpong -I 5 -v"
	"fi_rdm_pingpong -I 5 -v -U"
	"fi_rdm_tagged_pingpong -I 5"
	"fi_rdm_tagged_pingpong -I 5 -U"
	"fi_rdm_tagged_pingpong -I 5 -v"
	"fi_rdm_tagged_pingpong -I 5 -v -U"
	"fi_rdm_tagged_bw -I 5"
	"fi_rdm_tagged_bw -I 5 -U"
	"fi_rdm_tagged_bw -I 5 -v"
	"fi_rdm_tagged_bw -I 5 -v -U"
	"fi_dgram_pingpong -I 5"
)

standard_tests=(
	"fi_msg_pingpong"
	"fi_msg_pingpong -v"
	"fi_msg_pingpong -k"
	"fi_msg_pingpong -k -v"
	"fi_msg_bw"
	"fi_msg_bw -v"
	"fi_rma_bw -e msg -o write"
	"fi_rma_bw -e msg -o read"
	"fi_rma_bw -e msg -o writedata"
	"fi_rma_bw -e rdm -o write"
	"fi_rma_bw -e rdm -o write -U"
	"fi_rma_bw -e rdm -o read"
	"fi_rma_bw -e rdm -o read -U"
	"fi_rma_bw -e rdm -o writedata"
	"fi_rma_bw -e rdm -o writedata -U"
	"fi_rdm_atomic -o all -I 1000"
	"fi_rdm_atomic -o all -I 1000 -U"
	"fi_rdm_cntr_pingpong"
	"fi_multi_recv -e rdm"
	"fi_multi_recv -e msg"
	"fi_rdm_pingpong"
	"fi_rdm_pingpong -U"
	"fi_rdm_pingpong -v"
	"fi_rdm_pingpong -v -U"
	"fi_rdm_pingpong -k"
	"fi_rdm_pingpong -k -U"
	"fi_rdm_pingpong -k -v"
	"fi_rdm_pingpong -k -v -U"
	"fi_rdm_tagged_pingpong"
	"fi_rdm_tagged_pingpong -U"
	"fi_rdm_tagged_pingpong -v"
	"fi_rdm_tagged_pingpong -v -U"
	"fi_rdm_tagged_bw"
	"fi_rdm_tagged_bw -U"
	"fi_rdm_tagged_bw -v"
	"fi_rdm_tagged_bw -v -U"
	"fi_dgram_pingpong"
	"fi_dgram_pingpong -k"
)

unit_tests=(
	"fi_getinfo_test -s SERVER_ADDR GOOD_ADDR"
	"fi_av_test -g GOOD_ADDR -n 1 -s SERVER_ADDR"
	"fi_dom_test -n 2"
	"fi_eq_test"
	"fi_cq_test"
	"fi_mr_test"
	"fi_cntr_test"
)

complex_tests=(
	"fi_ubertest"
)

multinode_tests=(
	"fi_multinode -C msg"
	"fi_multinode -C rma"
	"fi_multinode_coll"
)

function errcho {
	>&2 echo $*
}

function print_border {
	printf "# "
	printf "%.0s-" {1..78}
	printf "\n"
}

function print_results {
	local test_name=$1
	local test_result=$2
	local test_time=$3
	local server_out_file=$4
	local server_cmd=$5
	local client_out_file=$6
	local client_cmd=$7

	if [ $VERBOSE -eq 0 ] ; then
		# print a simple, single-line format that is still valid YAML
		printf "%-70s%10s\n" "$test_exe:" "$test_result"
	else
		# Print a more detailed YAML format that is not a superset of
		# the non-verbose output.  See ofiwg/fabtests#259 for a
		# rationale.
		emit_stdout=0
		case $test_result in
			Pass*)
				[ $VERBOSE -ge 3 ] && emit_stdout=1
				;;
			Notrun|Excluded)
				[ $VERBOSE -ge 2 ] && emit_stdout=1
				;;
			Fail*)
				[ $VERBOSE -ge 1 ] && emit_stdout=1
				;;
		esac

		printf -- "- name:   %s\n" "$test_exe"
		printf -- "  result: %s\n" "$test_result"
		printf -- "  time:   %s\n" "$test_time"
		if [ $emit_stdout -eq 1 -a "$server_out_file" != "" ] ; then
			if [ "$server_cmd" != "" ] ; then
				printf -- "  server_cmd: %s\n" "$server_cmd"
			fi
			printf -- "  server_stdout: |\n"
			sed -e 's/^/    /' < $server_out_file
		fi
		if [ $emit_stdout -eq 1 -a "$client_out_file" != "" ] ; then
			if [ "$client_cmd" != "" ] ; then
				printf -- "  client_cmd: %s\n" "$client_cmd"
			fi
			printf -- "  client_stdout: |\n"
			sed -e 's/^/    /' < $client_out_file
		fi
	fi
}

function cleanup {
	${CLIENT_CMD} "ps -eo comm,pid | grep '^fi_' | awk '{print \$2}' | xargs kill -9" >& /dev/null
	${SERVER_CMD} "ps -eo comm,pid | grep '^fi_' | awk '{print \$2}' | xargs kill -9" >& /dev/null
	rm -f $c_outp $s_outp
}

function cleanup_and_exit {
	cleanup
	exit 1
}

# compute the duration in seconds between two integer values
# measured since the start of the UNIX epoch and print the result to stdout
function compute_duration {
	local -i s=$1
	local -i e=$2
	echo $(( $2 - $1))
}

function read_exclude_file {
	local excl_file=$1

	if [ ! -f $excl_file ]; then
		echo "Given exclusion file does not exist!"
		exit 1
	fi

	while read -r pattern || [[ -n "$pattern" ]]; do
		# Ignore patterns that are comments or just whitespaces
		ignore_pattern="#.*|^[\t ]*$"
		if [[ ! "$pattern" =~ $ignore_pattern ]]; then
			if [ -z "$file_excludes" ]; then
				file_excludes="$pattern"
			else
				file_excludes="${file_excludes},$pattern"
			fi
		fi
	done < "$excl_file"
}

function auto_exclude {
	local excl_file
	local name=$UTIL

	if [ -z $UTIL ]; then
		name=$CORE
	fi

	excl_file="./fabtests/test_configs/${name}/${name}.exclude"
	if [[ ! -f "$excl_file" ]]; then
		excl_file="./test_configs/${name}/${name}.exclude"
		if [[ ! -f "$excl_file" ]]; then
			excl_file="../test_configs/${name}/${name}.exclude"
			if [[ ! -f "$excl_file" ]]; then
				return
			fi
		fi
	fi

	read_exclude_file ${excl_file}
	cur_excludes=${file_excludes}
	file_excludes=""
}

function set_excludes {
	if [[ -n "$input_excludes" ]]; then
		cur_excludes=${input_excludes}
	fi

	if [[ -n "$file_excludes" ]]; then
		[[ -z "$cur_excludes" ]] && cur_excludes=${file_excludes} || \
			cur_excludes="${cur_excludes},${file_excludes}"
	fi

	if [[ -n "$cur_excludes" ]]; then
		return
	fi

	auto_exclude
}

function is_excluded {
	test_name=$1

	[[ -z "$cur_excludes" ]] && return 1

	IFS="," read -ra exclude_array <<< "$cur_excludes"
	for pattern in "${exclude_array[@]}"; do
		if [[ "$test_name" =~ $pattern ]]; then
			print_results "$test_exe" "Excluded" "0" "" ""
			skip_count+=1
			return 0
		fi
	done
	return 1
}

function unit_test {
	local test=$1
	local is_neg=$2
	local ret1=0
	local s_interface=$(eval "if [ $OOB -eq 1 ]; \
		then echo $GOOD_ADDR; \
		else echo $S_INTERFACE; \
		fi")
	local test_exe=$(echo "${test} -p \"$PROV\"" | \
	    sed -e "s/GOOD_ADDR/$GOOD_ADDR/g" -e "s/SERVER_ADDR/$s_interface/g")
	local start_time
	local end_time
	local test_time

	is_excluded "$test" && return

	start_time=$(date '+%s')

	cmd="${BIN_PATH}${test_exe}"
	${SERVER_CMD} "${EXPORT_ENV} $cmd" &> $s_outp &
	p1=$!

	wait $p1
	ret=$?

	end_time=$(date '+%s')
	test_time=$(compute_duration "$start_time" "$end_time")

	if [ $is_neg -eq 1 -a $ret -eq $FI_ENODATA ]; then
		# negative test passed
		ret=0
	elif [ $is_neg -eq 1 ]; then
		# negative test failed
		ret=1
	fi
	if [[ $STRICT_MODE -eq 0 && $ret -eq $FI_ENODATA || $ret -eq $FI_ENOSYS ]]; then
		print_results "$test_exe" "Notrun" "$test_time" "$s_outp" "$cmd"
		skip_count+=1
	elif [ $ret -ne 0 ]; then
		print_results "$test_exe" "Fail" "$test_time" "$s_outp" "$cmd"
		if [ $ret -eq 124 ]; then
			cleanup
		fi
		fail_count+=1
	else
		print_results "$test_exe" "Pass" "$test_time" "$s_outp" "$cmd"
		pass_count+=1
	fi
}

function cs_test {
	local test=$1
	local s_ret=0
	local c_ret=0
	local test_exe="${test} -p \"${PROV}\""
	local start_time
	local end_time
	local test_time

	is_excluded "$test" && return

	start_time=$(date '+%s')

	if [[ $OOB -eq 1 ]]; then
		s_arg="-E"
	else
		s_arg="-s $S_INTERFACE"
	fi
	s_cmd="${BIN_PATH}${test_exe} ${S_ARGS} $s_arg"
	${SERVER_CMD} "${EXPORT_ENV} $s_cmd" &> $s_outp &
	s_pid=$!
	sleep 1

	if [[ $OOB -eq 1 ]]; then
		c_arg="-E $S_INTERFACE"
	else
		c_arg="-s $C_INTERFACE $S_INTERFACE"
	fi
	c_cmd="${BIN_PATH}${test_exe} ${C_ARGS} $c_arg"
	${CLIENT_CMD} "${EXPORT_ENV} $c_cmd" &> $c_outp &
	c_pid=$!

	wait $c_pid
	c_ret=$?

	if [[ $c_ret -ne 0 ]] && ps -p $s_pid > /dev/null; then
	    if [[ $STRICT_MODE -eq 0 ]]; then
	        sleep 2
	    fi
	    kill -9 $s_pid 2> /dev/null
	fi

	wait $s_pid
	s_ret=$?

	end_time=$(date '+%s')
	test_time=$(compute_duration "$start_time" "$end_time")

	if [[ $STRICT_MODE -eq 0 && $s_ret -eq $FI_ENODATA && $c_ret -eq $FI_ENODATA ]] ||
	   [[ $STRICT_MODE -eq 0 && $s_ret -eq $FI_ENOSYS && $c_ret -eq $FI_ENOSYS ]]; then
		print_results "$test_exe" "Notrun" "$test_time" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
		skip_count+=1
	elif [ $s_ret -ne 0 -o $c_ret -ne 0 ]; then
		print_results "$test_exe" "Fail" "$test_time" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
		if [ $s_ret -eq 124 -o $c_ret -eq 124 ]; then
			cleanup
		fi
		fail_count+=1
	else
		print_results "$test_exe" "Pass" "$test_time" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
		pass_count+=1
	fi
}

function set_cfg_file {
	local cfg_file
	local parent=$UTIL
	local name=$CORE

	if [[ ! -z "$COMPLEX_CFG" ]]; then
		return
	fi

	if [ -z $UTIL ]; then
		parent=$CORE
		name=$1
	fi

	cfg_file="${PWD}/fabtests/test_configs/${parent}/${name}.test"
	if [[ ! -f "$cfg_file" ]]; then
		cfg_file="${PWD}/test_configs/${parent}/${name}.test"
		if [[ ! -f "$cfg_file" ]]; then
			return
		fi
	fi

	COMPLEX_CFG=${cfg_file}
}

function complex_test {
	local test=$1
	local config=$2
	local path=${PROV/;/\/}
	local test_exe="${test}"
	local s_ret=0
	local c_ret=0
	local start_time
	local end_time
	local test_time

	set_cfg_file $config
	if [[ -z "$COMPLEX_CFG" ]]; then
		is_excluded "$test" && return
	fi

	start_time=$(date '+%s')

	if [[ $FORK -eq 1 ]]; then
		opts="-f"
	else
		opts=""
	fi

	if [[ $OOB -eq 1 ]]; then
		opts+=" -E"
	fi

	s_cmd="${BIN_PATH}${test_exe} ${S_ARGS} -x $opts"
	FI_LOG_LEVEL=error ${SERVER_CMD} "${EXPORT_ENV} $s_cmd" &> $s_outp &
	s_pid=$!
	sleep 1

	c_cmd="${BIN_PATH}${test_exe} ${C_ARGS} -u "${COMPLEX_CFG}" $S_INTERFACE $opts"
	FI_LOG_LEVEL=error ${CLIENT_CMD} "${EXPORT_ENV} $c_cmd" &> $c_outp &
	c_pid=$!

	wait $c_pid
	c_ret=$?

	[[ c_ret -ne 0 ]] && kill -9 $s_pid

	wait $s_pid
	s_ret=$?

	end_time=$(date '+%s')
	test_time=$(compute_duration "$start_time" "$end_time")

	# case: config file doesn't exist or invalid option provided
	if [ $s_ret -eq 1 -o $c_ret -eq 1 ]; then
		print_results "$test_exe" "Notrun" "0" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
		cleanup
		skip_count+=1
		return
	# case: test didn't run becasue some error occured
	elif [ $s_ret -ne 0 -o $c_ret -ne 0 ]; then
		printf "%-50s%s\n" "$test_exe:" "Server returns $s_ret, client returns $c_ret"
		print_results "$test_exe" "Fail [$f_cnt/$total]" "$test_time" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
                cleanup
                fail_count+=1
	else
		local f_cnt=$(cat $c_outp | awk -F': ' '/ENOSYS|ERROR/ {total += $2} END {print total}')
		local s_cnt=$(cat $c_outp | awk -F': ' '/Success/ {total += $2} END {print total}')
		local total=$(cat $c_outp | awk -F': ' '/Success|ENODATA|ENOSYS|ERROR/ {total += $2} END {print total}')
		if [ $f_cnt -eq 0 ]; then
			print_results "$test_exe" "Pass [$s_cnt/$total]" "$test_time" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
			pass_count+=1
		else
			print_results "$test_exe" "Fail [$f_cnt/$total]" "$test_time" "$s_outp" "$s_cmd" "$c_outp" "$c_cmd"
			cleanup
			fail_count+=1
		fi
	fi
}

function multinode_test {
	local test="$1"
	local s_ret=0
	local c_ret=0
	local c_out_arr=()
	local num_procs=$2
	local test_exe="${test} -n $num_procs -p \"${PROV}\""
	local c_out
	local start_time
	local end_time
	local test_time

	is_excluded "$test" && return

	start_time=$(date '+%s')

	s_cmd="${BIN_PATH}${test_exe} ${S_ARGS} -s ${S_INTERFACE}"
	${SERVER_CMD} "${EXPORT_ENV} $s_cmd" &> $s_outp &
	s_pid=$!
	sleep 1

	c_pid_arr=()
	for ((i=1; i<num_procs; i++))
	do
		local c_out=$(mktemp fabtests.c_outp${i}.XXXXXX)
		c_cmd="${BIN_PATH}${test_exe} ${S_ARGS} -s ${S_INTERFACE}"
		${CLIENT_CMD} "${EXPORT_ENV} $c_cmd" &> $c_out &
		c_pid_arr+=($!)
		c_out_arr+=($c_out)
	done

	for pid in ${c_pid_arr[*]}; do
		wait $pid
		c_ret=($?)||$c_ret
	done

	if [[ $c_ret -ne 0 ]] && ps -p $s_pid > /dev/null; then
	    if [[ $STRICT_MODE -eq 0 ]]; then
	        sleep 2
	    fi
	    kill -9 $s_pid 2> /dev/null
	fi

	wait $s_pid
	s_ret=$?
	echo "server finished"

	end_time=$(date '+%s')
	test_time=$(compute_duration "$start_time" "$end_time")

	pe=1
	if [[ $STRICT_MODE -eq 0 && $s_ret -eq $FI_ENODATA && $c_ret -eq $FI_ENODATA ]] ||
	   [[ $STRICT_MODE -eq 0 && $s_ret -eq $FI_ENOSYS && $c_ret -eq $FI_ENOSYS ]]; then
		print_results "$test_exe" "Notrun" "$test_time" "$s_outp" "$s_cmd" "" "$c_cmd"
		for c_out in "${c_out_arr[@]}"
		do
			printf -- "  client_stdout $pe: |\n"
			sed -e 's/^/    /' < $c_out
			pe=$((pe+1))
		done
		skip_count+=1
	elif [ $s_ret -ne 0 -o $c_ret -ne 0 ]; then
		print_results "$test_exe" "Fail" "$test_time" "$s_outp" "$s_cmd" "" "$c_cmd"
		for c_out in "${c_out_arr[@]}"
		do
			printf -- "  client_stdout $pe: |\n"
			sed -e 's/^/    /' < $c_out
			pe=$((pe+1))
		done
		if [ $s_ret -eq 124 -o $c_ret -eq 124 ]; then
			cleanup
		fi
		fail_count+=1
	else
		print_results "$test_exe" "Pass" "$test_time" "$s_outp" "$s_cmd" "" "$c_cmd"
		for c_out in "${c_out_arr[@]}"
		do
			printf -- "  client_stdout $pe: |\n"
			sed -e 's/^/    /' < $c_out
			pe=$((pe+1))
		done
		pass_count+=1
	fi
}

function set_core_util {
	prov_arr=$(echo $PROV | tr ";" " ")
	CORE=""
	UTIL=""
	for p in $prov_arr; do
		if [[ -z $CORE ]]; then
			CORE=$p
		else
			UTIL=$p
		fi
	done
}

function main {
	skip_count=0
	pass_count=0
	fail_count=0
	local complex_type="quick"

	set_core_util
	set_excludes


	if [[ $1 == "quick" ]]; then
		local -r tests="unit functional short"
	elif [[ $1 == "verify" ]]; then
		local -r tests="complex"
		complex_type=$1
	else
		local -r tests=$(echo $1 | sed 's/all/unit,functional,standard,complex,multinode/g' | tr ',' ' ')
		if [[ $1 == "all" || $1 == "complex" ]]; then
			complex_type="all"
		fi
	fi

	if [ $VERBOSE -eq 0 ] ; then
		printf "# %-68s%10s\n" "Test" "Result"
		print_border
	fi

	for ts in ${tests}; do
	case ${ts} in
		unit)
			for test in "${unit_tests[@]}"; do
				unit_test "$test" "0"
			done

			if [ $SKIP_NEG -eq 0 ] ; then
				for test in "${neg_unit_tests[@]}"; do
					unit_test "$test" "1"
				done
			fi
		;;
		functional)
			for test in "${functional_tests[@]}"; do
				cs_test "$test"
			done
		;;
		short)
			for test in "${short_tests[@]}"; do
				cs_test "$test"
			done
		;;
		standard)
			for test in "${standard_tests[@]}"; do
				cs_test "$test"
			done
		;;
		complex)
			for test in "${complex_tests[@]}"; do
				complex_test $test $complex_type
			done
		;;
		multinode)
			for test in "${multinode_tests[@]}"; do
					multinode_test "$test" 3
			done
		;;
		*)
			errcho "Unknown test set: ${ts}"
			exit 1
		;;
	esac
	done

	total=$(( $pass_count + $fail_count ))

	print_border

	printf "# %-50s%10d\n" "Total Pass" $pass_count
	printf "# %-50s%10d\n" "Total Notrun/Excluded" $skip_count
	printf "# %-50s%10d\n" "Total Fail" $fail_count

	if [[ "$total" > "0" ]]; then
		printf "# %-50s%10d\n" "Percentage of Pass" $(( $pass_count * 100 / $total ))
	fi

	print_border

	cleanup
	total_failures+=$fail_count
}

function usage {
	errcho "Usage:"
	errcho "  $0 [OPTIONS] [provider] [host] [client]"
	errcho
	errcho "Run fabtests using provider between host and client (default"
	errcho "'sockets' provider in loopback-mode).  Report pass/fail/notrun status."
	errcho
	errcho "Options:"
	errcho -e " -g\tgood IP address from <host>'s perspective (default $GOOD_ADDR)"
	errcho -e " -v\tprint output of failing"
	errcho -e " -vv\tprint output of failing/notrun"
	errcho -e " -vvv\tprint output of failing/notrun/passing"
	errcho -e " -t\ttest set(s): all,quick,unit,functional,standard,short,complex (default quick)"
	errcho -e " -e\texclude tests: comma delimited list of test names /
			 regex patterns e.g. \"dgram,rma.*write\""
	errcho -e " -E\texport provided variable name and value to ssh client and server processes.
			 options must of of the form '-E var=value'"
	errcho -e " -U\trun fabtests with FI_DELIVERY_COMPLETE set"
	errcho -e " -f\texclude tests file: File containing list of test names /
			 regex patterns to exclude (one per line)"
	errcho -e " -N\tskip negative unit tests"
	errcho -e " -p\tpath to test bins (default PATH)"
	errcho -e " -c\tclient interface"
	errcho -e " -s\tserver/host interface"
	errcho -e " -u\tconfigure option for complex tests"
	errcho -e " -T\ttimeout value in seconds"
	errcho -e " -S\tStrict mode: -FI_ENODATA, -FI_ENOSYS errors would be treated as failures instead of skipped/notrun"
	errcho -e " -C\tAdditional client test arguments: Parameters to pass to client fabtests"
	errcho -e " -L\tAdditional server test arguments: Parameters to pass to server fabtests"
	errcho -e " -b\tenable out-of-band address exchange over the default port"
	exit 1
}

while getopts ":vt:p:g:e:f:c:s:u:T:C:L:NRSbkE:" opt; do
case ${opt} in
	t) TEST_TYPE=$OPTARG
	;;
	v) VERBOSE+=1
	;;
	p) BIN_PATH="${OPTARG}/"
	;;
	g) GOOD_ADDR=${OPTARG}
	;;
	f) read_exclude_file ${OPTARG}
	;;
	e) [[ -z "$input_excludes" ]] && input_excludes=${OPTARG} || \
		input_excludes="${input_excludes},${OPTARG}"
	;;
	c) C_INTERFACE=${OPTARG}
	;;
	s) S_INTERFACE=${OPTARG}
	;;
	u) COMPLEX_CFG=${OPTARG}
	;;
	T) TIMEOUT_VAL=${OPTARG}
	;;
	N) SKIP_NEG+=1
	;;
	R)
	;;
	S) STRICT_MODE=1
	;;
	b) OOB=1
	;;
	k) FORK=1
	;;
	C) C_ARGS="${OPTARG}"
	;;
	L) S_ARGS="${OPTARG}"
	;;
	E)
	delimiter="="
	value=${OPTARG#*$delimiter}
	var=${OPTARG:0:$(( ${#OPTARG} - ${#value} - ${#delimiter} ))}
	EXPORT_STRING="export $var=\"$value\""
	if [[ -z $EXPORT_ENV ]] ; then
		EXPORT_ENV="$EXPORT_STRING ;"
	else
		EXPORT_ENV="$EXPORT_ENV $EXPORT_STRING ;"
	fi
	;;
	:|\?) usage
	;;
esac

done

# base ssh command
declare bssh="ssh -n -o StrictHostKeyChecking=no -o ConnectTimeout=2 -o BatchMode=yes"
if [ -z "$(which timeout 2> /dev/null)" ]; then
	# forego timeout
	declare SERVER_CMD="eval"
	declare CLIENT_CMD="eval"
else
	declare SERVER_CMD="eval timeout ${TIMEOUT_VAL}"
	declare CLIENT_CMD="eval timeout ${TIMEOUT_VAL}"
	bssh="timeout ${TIMEOUT_VAL} ${bssh}"
fi

# shift past options
shift $((OPTIND-1))

if [[ $# -ge 4 ]]; then
	usage
fi

if [[ $# -ge 1 ]]; then
	PROV=$1
fi

if [[ $# -ge 2 ]]; then
	SERVER=$2
	SERVER_CMD="${bssh} ${SERVER}"
fi

if [[ $# -ge 3 ]]; then
	CLIENT=$3
	CLIENT_CMD="${bssh} ${CLIENT}"
fi

[ -z $C_INTERFACE ] && C_INTERFACE=$CLIENT
[ -z $S_INTERFACE ] && S_INTERFACE=$SERVER
[ -z $GOOD_ADDR ] && GOOD_ADDR=$S_INTERFACE

if [[ -z $PROV ]]; then
	PROV="tcp"
	main ${TEST_TYPE}
	PROV="udp"
	main ${TEST_TYPE}
else
	main ${TEST_TYPE}
fi

exit $total_failures

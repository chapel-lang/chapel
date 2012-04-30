#/bin/bash

# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2012  inria
#
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

mode="chain" # By default, let's do everything !
results="./results" # TODO Add an option for this
scripts_dir=
target=
spatch=`which spatch`

if [ $spatch = "" ]
then
	echo "Could not find spatch."
	exit 1;
fi

delete_if_empty()
{
	test -s $1 || rm $1
}

create_directories()
{
	if [ $1 == "chain" ]
	then
		# XXX mkdir -p ${results}/{context,org,patch,report} ?
		mkdir -p ${results}/context
		mkdir -p ${results}/org
		mkdir -p ${results}/patch
		mkdir -p ${results}/report
	else
		mkdir -p ${results}/$1
	fi
}

run_script()
{
	script=$1
	mode=$2

	# Make sure the script explicitely defines this virtual rule.
	grep "^virtual ${mode}" $script > /dev/null || return;

	output_file=`basename ${script}`
	output_file=${results}/${mode}/${output_file%cocci}${mode}
	error_file=${output_file%${mode}}err

	options="-very_quiet"

	echo "Running ${script} with mode ${mode}."
	# Seems like we do not need to specify -dir.
	# XXX Try to use -o
	$spatch $script $target -D $mode ${options} > ${output_file} 2>${error_file}

	# Nobody cares about empty files
	delete_if_empty ${output_file}
	delete_if_empty ${error_file}
}

run_scripts ()
{
	mode=$1
	for script in `find $scripts_dir -name "*.cocci"`
	do
		# XXX This is ugly.
		if [ $mode = "context" -o $mode = "chain" ]
		then
			run_script ${script} "context"
		fi
		if [ $mode = "org" -o $mode = "chain" ]
		then
			run_script ${script} "org"
		fi
		if [ $mode = "patch" -o $mode = "chain" ]
		then
			run_script ${script} "patch"
		fi
		if [ $mode = "report" -o $mode = "chain" ]
		then
			run_script ${script} "report"
		fi
	done
}

usage ()
{
	echo "\
$0 [OPTIONS]

-h               Print this help
-m <mode>        Mode to use : chain, context, org, patch, report
-s <scripts_dir> Path to the directory where the cocci scripts are stored
-t <target>      File or directory to parse
"
}

while getopts ":hm:s:t:" opt;
do
	case $opt in
	h)
		usage
		exit 0;
		;;
	m)
		mode=$OPTARG;
		;;
	s)
		scripts_dir=$OPTARG;
		;;
	t)
		target="$target $OPTARG";
		;;
	\?)
		echo "Invalid option -$OPTARG"
		exit 1
		;;	
	\:)
		echo "Option -$OPTARG requires an argument"
		exit 1
		;;
	esac
done

# Is that {} thing portable ?
test -n "$target" || { echo "-t <target> must be specified" && exit 1; }
test -n "$scripts_dir" || { echo "-s <scripts_dir> must be specified" && exit 1; }

create_directories $mode
if [ "$mode" = "chain" ]
then
	run_scripts "context"
	run_scripts "org"
	run_scripts "patch"
	run_scripts "report"
else
	run_scripts $mode
fi
exit 0

#! /bin/ksh
#
#   $Source: /var/local/cvs/gasnet/sci-conduit/contrib/gasnetrun_sci.ksh,v $
#     $Date: 2004/08/26 04:54:03 $
# $Revision: 1.3 $
# Description: 
# Terms of use are as specified in license.txt
# Burt Gordon, High-performance Computing & Simulation (HCS) Lab
# June 2003
# This script reads the contents of the "machines" file and 
# attempts to rsh into each machine and begin SCI initialization.
# 
#
#print "Script started"

#function to print a help screen
PrintHelp() 
{

 print "Usage:"
 print "	./scistart [options] <program_name> [program_args]"
 print ""		
 print "	<program_name>	: Program to be run on each node. Assumed to be "
 print "			 a globally accessible location. If not fully "
 print "			 qualified, assumed to be in the current directory."
 print "      NOTE: This script must be run from the machine which will act as gasnet"
 print "            node 0. This will always be the first machine listed in the "
 print "            machines.sci or the jobs file, whichever is used."
 print ""
 print "Options:"
 print "	-n <number>		: number of nodes to use."
 print "			If number exceeds the total number of nodes listed in the machines file,"
 print "			then program_name is started once on each of the nodes in the file. If "
 print "			-n option is not used, only one copy of program_name will be started."
 print ""
 print "	-machines <file>	: absolute location of machines file. If not specified, equal to "
 print "				machines.sci in current directory"
 print ""
 print "	-jobs <job_file>	: Complete path to the list of nodes to use, "
 print "			nodes must exist in the machines file"
 print ""
 print "	-help			: this message"
 print ""
}

#go through each of the arguments to the script
while [ $1 ]
do
	# case is used to determine the proper actions to be taken
    case $1 in
    -n) 
	NUMBER_NODES=$2
	shift 2
	;;
    -machines) 
	MFILE=$2
	shift 2
	;;
    -jobs) 
	JOB_FILE=$2
	shift 2
	;;
    -help | -h) 
	PrintHelp
	exit 1
	;;
     # everything else is a program name followed by its arguments
     *) 
	PROG=$1
	shift
	while [ $1 ]
	do
	 ARGS="$ARGS $1"
	 shift
	done
     esac
done

#Need to get the directory info from the program name

if [ $PROG ]
 then
	DIR=${PROG%/*}
	PROG=${PROG##*/}
 else
	print "ERROR: Program name is not specified. Exiting..."
	print ""
	PrintHelp
	exit 1
fi

#print "Directory is: $DIR"

if [ $PROG == $DIR ]
then
	DIR=$PWD
fi

#print "Directory is: $DIR"

if [ $MFILE ]
 then
	X=1
 else
	if [ -a machines.sci ]
	then
		MFILE=machines.sci
	else
	   print "Cannot find machines file"
	   exit 1
	fi
fi

if [  $NUMBER_NODES ]
 then
	X=1
 else
	NUMBER_NODES=1
fi

#print "Program Name is : $PROG"
#print "Arguments are : $ARGS"
#print "N is equal to : $NUMBER_NODES"
#print "Machines file now equal to : $MFILE"
#print "Jobs files is now: $JOB_FILE"


# Now read Machines file and get information

#print "Reading host info"

HOSTS=`cat $MFILE | awk 'NF==0 {exit} {print $1}'`
SCI_IDS=`cat $MFILE | awk 'NF==0 {exit} {print $2}'`

#print "Hosts are :"
#print "$HOSTS"
#print "SCI IDs are :"
#print "$SCI_IDS"
#print ""

if [ $JOB_FILE ]
 then
	#print "Checking to see if all nodes are found"

	JOBS=`cat $JOB_FILE | awk 'NF==0 {exit} {print $1}'`

	#print "Jobs: "
	#print "$JOBS"

	#check if all these nodes are in the machines file
	all=1  # as far as we know everything is fine
	for i in $JOBS
	 do
	   # variables to check status of things 
	   HERE=0    # used to determine if name matches
	   k=0       #used to find sci id of matched name
	   l=0
	   for j in $HOSTS 
	    do
	       if [[ $i = $j ]]
	       then
		  HERE=1
		  for id in $SCI_IDS
		   do
			#print "ID is : $id"
			#print "k is: $k, l is :$l"
			if (( $k == $l ))
			then
			    NODE_IDS="$NODE_IDS$id \n"
			    #print "SCI Id is: $id"
			    break
			 else
			    k=`expr $k + 1`
			 fi
		   done
		  #print "Node : $j, SCI ID: $id"
	       fi
	       l=`expr $l + 1`

	   done

	   if (( $HERE != 1 ))
	    then
	      all=0 #there is a name in jobs that doesn't match anybody in machines
	      break
	   fi
	 done
	
	#print "finished for loops, all is: $all"
	
	 if (( $all == 0))
	  then
	     print "ERROR: Not all nodes listed in Jobs file found in machines file"
	     exit 1
	  fi
         #otherwise everything is ready, so use the nodes listed in the jobs file
	HOSTS=$JOBS
	SCI_IDS=$NODE_IDS
		#print "Hosts are :"
		#print "$HOSTS"
		#print ""
		#print "IDs are :"
		#print "$NODE_IDS"
   
fi



#Now set up file info based on job node names and sci ids
#one from jobs, one from ids, ";" after each pair
# this is used to make sure we know how many
# nodes there are and what their GASNet ids are
#GASNet ids are mapped from 0 to n starting with the
# top of the file list going to the end
i=0
for h in $HOSTS
do
  j=0
  #print "h is :$h,"
  for k in $SCI_IDS
  do 
	k=${k##*\n} #cut away any leading newline characters
	if (( $i == $j ))
	then
		if (( $i < $NUMBER_NODES )) #make sure we have the correct number
		then
			#print "h is :$h, k is : $k,"
			FILE="$FILE $k;" #just place SCI Ids, don't need node names
		fi
	fi
	j=`expr $j + 1`

	#print "i is: $i, and j is:$j"
  done
  i=`expr $i + 1`	
done

#print "Hosts and ids are:"
#print "$HOSTS :: $FILE"

#write the file information into a global location near the program
copy_dir=$PWD


cd $DIR
rm -f gasnet_nodes.sci
if (( $i < $NUMBER_NODES ))
then
cat > gasnet_nodes.sci << EOF
$i
$FILE
EOF
else
cat > gasnet_nodes.sci << EOF
$NUMBER_NODES
$FILE
EOF
fi


# now with that information, we need to rsh into every machine and start 
# the initialization program. 

#First we need to extract the first machine in the list, this will be the
# gasnet node 0. This machine should also be equal to the hostname of the
# machine on which this script is running. We will not rsh into this machine,
# rather we will start it last and have the script wait until it has completed
# the program before the script then ends.

TEMP_HOSTS=$HOSTS
HOSTS=""

#print " Temp_Hosts are:"
#print "$TEMP_HOSTS"
#print " Hosts are now: $HOSTS."

i=0
for h in $TEMP_HOSTS
 do
	if (( $i < 1 ))
	 then
		FIRST=$h
		i=`expr $i + 1`
		#print "i equal to $i"
	 else
		HOSTS="$HOSTS$h "	
	fi
 done

# don't make this machine have to be the same as gasnet 0
# THISNAME=`hostname`
# print "Hostname is: $THISNAME"

#check to see if the names match
# if [ $THISNAME != $FIRST ]
#  then
#	print "Hostname returned: $THISNAME, the first node in the list is : $FIRST"
#	print " The two names must be equal and this script needs to be started from"
#	print " the machine which will act as gasnet node #0. Exiting..."
#	exit
# fi

# print "Hosts are:"
# print "$HOSTS"
# print "Node 0 is: $FIRST"

# need the & to run rsh as backgroud or else the script must wait
# for the program to terminate before moving on to next line

# for loop to rsh to each machine in the file
#need to fix this so node 0 goes last and the script waits for it to terminate 
#  the program

print "Beginning program on host(s)"

i=1
for h in $HOSTS
 do
	if (( $i < $NUMBER_NODES ))
	 then
		rsh $h "cd $DIR; $PROG $ARGS ; exit;"&
		#print "Rsh-ed into $h"
		i=`expr $i + 1`
		#print "i equal to $i"
	 else
		break	
	fi
 done
#print "Finished with rsh"

#start the program on the first host
rsh $FIRST "cd $DIR; $PROG $ARGS ; exit;"


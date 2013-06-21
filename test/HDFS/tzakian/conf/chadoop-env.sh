#!/bin/bash

#export JAVA_HOME=/usr/lib/jvm/java-openjdk
#export HADOOP_HOME=/usr/lib/hadoop
#export HBASE_HOME=/usr/lib/hbase
#export HIVE_HOME=/usr/lib/hive
#export LD_LIBRARY_PATH=/usr/lib/jvm/java-openjdk/jre/lib/amd64/server
#export CLASSPATH=$HADOOP_HOME/conf:$HADOOP_HOME/hadoop-ant.jar:$HADOOP_HOME/hadoop-core.jar:$HADOOP_HOME/hadoop-examples.jar:$HADOOP_HOME/hadoop-test.jar:$HADOOP_HOME/hadoop-tools.jar:$HADOOP_HOME/lib/ant-contrib-1.0b3.jar:$HADOOP_HOME/lib/asm-3.2.jar:$HADOOP_HOME/lib/aspectjrt-1.6.5.jar:$HADOOP_HOME/lib/aspectjtools-1.6.5.jar:$HADOOP_HOME/lib/commons-cli-1.2.jar:$HADOOP_HOME/lib/commons-codec-1.4.jar:$HADOOP_HOME/lib/commons-daemon-1.0.1.jar:$HADOOP_HOME/lib/commons-el-1.0.jar:$HADOOP_HOME/lib/commons-httpclient-3.1.jar:$HADOOP_HOME/lib/commons-io-2.1.jar:$HADOOP_HOME/lib/commons-lang-2.4.jar:$HADOOP_HOME/lib/commons-logging-1.0.4.jar:$HADOOP_HOME/lib/commons-logging-api-1.0.4.jar:$HADOOP_HOME/lib/commons-net-3.1.jar:$HADOOP_HOME/lib/core-3.1.1.jar:$HADOOP_HOME/lib/guava-r09-jarjar.jar:$HADOOP_HOME/lib/hadoop-fairscheduler-0.20.2-cdh3u5.jar:$HADOOP_HOME/lib/hsqldb-1.8.0.10.jar:$HADOOP_HOME/lib/jackson-core-asl-1.5.2.jar:$HADOOP_HOME/lib/jackson-mapper-asl-1.5.2.jar:$HADOOP_HOME/lib/jasper-compiler-5.5.12.jar:$HADOOP_HOME/lib/jasper-runtime-5.5.12.jar:$HADOOP_HOME/lib/jersey-core-1.8.jar:$HADOOP_HOME/lib/jersey-json-1.8.jar:$HADOOP_HOME/lib/jersey-server-1.8.jar:$HADOOP_HOME/lib/jets3t-0.6.1.jar:$HADOOP_HOME/lib/jetty-6.1.26.cloudera.1.jar:$HADOOP_HOME/lib/jetty-servlet-tester-6.1.26.cloudera.1.jar:$HADOOP_HOME/lib/jetty-util-6.1.26.cloudera.1.jar:$HADOOP_HOME/lib/jsch-0.1.42.jar:$HADOOP_HOME/lib/junit-4.5.jar:$HADOOP_HOME/lib/kfs-0.2.2.jar:$HADOOP_HOME/lib/log4j-1.2.15.jar:$HADOOP_HOME/lib/mockito-all-1.8.2.jar:$HADOOP_HOME/lib/oro-2.0.8.jar:$HADOOP_HOME/lib/servlet-api-2.5-20081211.jar:$HADOOP_HOME/lib/servlet-api-2.5-6.1.14.jar:$HADOOP_HOME/lib/slf4j-api-1.4.3.jar:$HADOOP_HOME/lib/slf4j-log4j12-1.4.3.jar:$HADOOP_HOME/lib/xmlenc-0.52.jar

#export CLASSPATH=/conf$HADOOP_HOME/hadoop-ant-1.1.2.jar$HADOOP_HOME/hadoop-core-1.1.2.jar$HADOOP_HOME/hadoop-examples-1.1.2.jar$HADOOP_HOME/hadoop-test-1.1.2.jar$HADOOP_HOME/hadoop-tools-1.1.2.jar$HADOOP_HOME/lib/ant-contrib-1.0b3.jar$HADOOP_HOME/lib/asm-3.2.jar$HADOOP_HOME/lib/aspectjrt-1.6.11.jar$HADOOP_HOME/lib/aspectjtools-1.6.11.jar$HADOOP_HOME/lib/commons-cli-1.2.jar$HADOOP_HOME/lib/commons-codec-1.4.jar$HADOOP_HOME/lib/commons-daemon-1.0.1.jar$HADOOP_HOME/lib/commons-el-1.0.jar$HADOOP_HOME/lib/commons-httpclient-3.0.1.jar$HADOOP_HOME/lib/commons-io-2.1.jar$HADOOP_HOME/lib/commons-lang-2.4.jar$HADOOP_HOME/lib/commons-logging-1.1.1.jar$HADOOP_HOME/lib/commons-logging-api-1.0.4.jar$HADOOP_HOME/lib/commons-net-3.1.jar$HADOOP_HOME/lib/core-3.1.1.jar$HADOOP_HOME/lib/hadoop-fairscheduler-1.1.2.jar$HADOOP_HOME/lib/hsqldb-1.8.0.10.jar$HADOOP_HOME/lib/jackson-core-asl-1.8.8.jar$HADOOP_HOME/lib/jackson-mapper-asl-1.8.8.jar$HADOOP_HOME/lib/jasper-compiler-5.5.12.jar$HADOOP_HOME/lib/jasper-runtime-5.5.12.jar$HADOOP_HOME/lib/jersey-core-1.8.jar$HADOOP_HOME/lib/jersey-json-1.8.jar$HADOOP_HOME/lib/jersey-server-1.8.jar$HADOOP_HOME/lib/jets3t-0.6.1.jar$HADOOP_HOME/lib/jetty-6.1.26.jar$HADOOP_HOME/lib/jetty-util-6.1.26.jar$HADOOP_HOME/lib/jsch-0.1.42.jar$HADOOP_HOME/lib/junit-4.5.jar$HADOOP_HOME/lib/kfs-0.2.2.jar$HADOOP_HOME/lib/log4j-1.2.15.jar$HADOOP_HOME/lib/mockito-all-1.8.5.jar$HADOOP_HOME/lib/oro-2.0.8.jar$HADOOP_HOME/lib/servlet-api-2.5-20081211.jar$HADOOP_HOME/lib/slf4j-api-1.4.3.jar$HADOOP_HOME/lib/slf4j-log4j12-1.4.3.jar$HADOOP_HOME/lib/xmlenc-0.52.jar
#pushd ~/chapel && source util/setchplenv.bash; popd
#export CHPL_COMM=gasnet

# This script constantly interferes with rsync by printing to the console at login
#
# bash shell script to set the Chapel environment variables
# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
#oldDir=`pwd`
#cd ~/chapel
#if [ -d "util" ] && [ -d "compiler" ] && [ -d "runtime" ] && [ -d "modules" ]
#   then
#      MYPATH=`./util/config/fixpath "$PATH" :`
#      MYMANPATH=`./util/config/fixpath "$MANPATH" :`
#      if [ -z "$MYPATH" ]
#        then
#          echo "Error running ./util/config/fixpath";
#        else
#          #echo -n "Setting CHPL_HOME "
#          export CHPL_HOME=$PWD
#          #echo "to $CHPL_HOME"
#
#          #echo -n "Setting CHPL_HOST_PLATFORM "
#          export CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/chplenv/platform`
#          #echo "to $CHPL_HOST_PLATFORM"
#
#          #echo -n "Updating PATH to include "
#          export PATH="$MYPATH":"$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM:"$CHPL_HOME"/util
#          #echo "$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
#          #echo    "                     and ""$CHPL_HOME"/util
#
#          #echo -n "Updating MANPATH to include "
#          export MANPATH="$MYMANPATH":"$CHPL_HOME"/man
#          #echo "$CHPL_HOME"/man
#        fi
#   else
#      echo "Error: util/setchplenv must be sourced from within the chapel root directory"
#fi
#cd $oldDir

source ~/chadoop/multiloc/setenv.multiloc-ssh.bash

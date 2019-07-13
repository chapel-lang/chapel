#!/usr/bin/env bash
#
# Test HDFS and Curl package modules

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-quickstart.bash

export HADOOP_HOME=/data/cf/chapel/hadoop/$HOSTNAME
export JAVA_HOME=/usr/lib64/jvm/jre
export CLASSPATH=$(${HADOOP_HOME}/bin/hadoop classpath --glob)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HADOOP_HOME/lib/native:$JAVA_HOME/lib:$JAVA_HOME/lib/amd64/server

# remove storage directory root
rm -rf /tmp/hadoop-chapelu/

#reformat hdfs filesystem
$HADOOP_HOME/bin/hdfs namenode -format test
#start hdfs
$HADOOP_HOME/sbin/start-dfs.sh
$HADOOP_HOME/bin hdfs dfsadmin -safemode leave


export CHPL_NIGHTLY_TEST_CONFIG_NAME="networking-packages"
export CHPL_NIGHTLY_TEST_DIRS="library/packages/Curl library/packages/HDFS"

$CWD/nightly -cron ${nightly_args}

#stop hdfs
$HADOOP_HOME/sbin/stop-dfs.sh

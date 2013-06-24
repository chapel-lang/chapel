#!/bin/bash

echo "Setting multiloc environment"
source multiloc/setenv.multiloc-ssh.bash

echo "Syncing ~/chadoop/ ~/chapel/ and ~/.bashrc across cluster"
numHosts=0
for cluster_host in ${SSH_SERVERS}
do
	((numHosts += 1))
	if [ $cluster_host != `hostname` ]; then
		rsync -av --delete -e "ssh -l admin" ~/chadoop/ ${cluster_host}:~/chadoop/
		rsync -av --delete -e "ssh -l admin" ~/chapel/ ${cluster_host}:~/chapel/
		scp ~/.bashrc ${cluster_host}:~/.bashrc
	fi
done

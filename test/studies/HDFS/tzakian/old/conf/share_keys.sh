#!/bin/bash
source ~/chadoop/multiloc/setenv.multiloc-ssh.bash

for cluster_host in ${SSH_SERVERS}
do
	scp ~/.ssh/id_rsa.pub ${cluster_host}:~/tzakian_id_rsa.pub
	ssh ${cluster_host} "cat tzakian_id_rsa.pub >> .ssh/authorized_keys"
done


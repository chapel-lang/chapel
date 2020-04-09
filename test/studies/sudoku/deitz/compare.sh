#!/bin/sh

tr -d '\n' < $1 | sed s/\\\ //g > out1
tr -d '\n' < $2 | sed s/\\\ //g > out2
diff out1 out2

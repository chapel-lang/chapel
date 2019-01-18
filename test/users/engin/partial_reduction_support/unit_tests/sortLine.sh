#! /bin/sh
echo $(for i in "$@"; do echo $i; done | sort -n)

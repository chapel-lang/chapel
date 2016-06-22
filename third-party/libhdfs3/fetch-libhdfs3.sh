#!/bin/sh

if [ -d libhdfs3 ]
then

echo libhdfs3 already fetched!

else 
echo Fetching libhdfs3
git clone https://github.com/PivotalRD/libhdfs3.git

fi

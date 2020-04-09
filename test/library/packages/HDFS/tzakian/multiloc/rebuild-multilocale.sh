pushd $CHPL_HOME

pushd third-party/gasnet
make clobber all
popd # $CHPL_HOME

pushd runtime
make clean all
popd # $CHPL_HOME

popd # .

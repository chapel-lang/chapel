use Path;

var x1:string = "$CHPL_HOME/bin/${CHPL_HOST_PLATFORM}";
assert(expandVars(x1) == CHPL_HOME+"/bin/"+CHPL_HOST_PLATFORM);

var x2:string = "bin/$CHPL_COMM/$CHPL_COMM_SUBSTRATE/";
assert(expandVars(x2) == "bin/"+CHPL_COMM+"/"+CHPL_COMM_SUBSTRATE+"/");

use Path;

var x:string = "$CHPL_HOME/bin/${CHPL_HOST_PLATFORM}";
assert(expandVars(x) == CHPL_HOME+"/bin/"+CHPL_HOST_PLATFORM);
use Path;

var x1:string = "$CHPL_HOME/bin/${TEST_HOST_PLATFORM}";
assert(expandVars(x1) == CHPL_HOME+"/bin/"+CHPL_HOST_PLATFORM);

var x2:string = "bin/$ABCD/$ABCD_DEF";
assert(expandVars(x2) == "bin/wind/typhoon");

var x3:string = "A$";
assert(expandVars(x3) == "A$");

var x4:string = "A${";
assert(expandVars(x4) == "A${");

var x5:string = "${FOO${RIDGE}}";
assert(expandVars(x5) == "${FOO${RIDGE}}");

var x6:string = "${LAKE}/someText/$RIDGE";
assert(expandVars(x6) == "michigan/someText/BAR");

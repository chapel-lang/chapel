config var dummy = "dummy"; // to make sure that there's a section for this
                            // module in --help

@unstable("configVar is unstable.")
config var configVar = "default";

@unstable("configConst is unstable.")
config const configConst = "default";

writeln(dummy);
writeln(configVar);
writeln(configConst);

@unstable("privateConfigVar is unstable.")
private config var privateConfigVar = "default";

writeln(privateConfigVar);

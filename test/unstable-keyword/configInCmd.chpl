@unstable("configVar is unstable.")
config var configVar = "default";

@unstable("configConst is unstable.")
config var configConst = "default";

@unstable("configParam is unstable.")
config var configParam = "default";

writeln(configVar);
writeln(configConst);
writeln(configParam);

@unstable("privateConfigVar is unstable.")
private config var privateConfigVar = "default";

writeln(privateConfigVar);

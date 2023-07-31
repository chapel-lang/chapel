@deprecated("privateConfigVarDepr is deprecated.")
private config var privateConfigVarDepr = "default";

@deprecated("configVarDepr is deprecated.")
config var configVarDepr = "default";

@unstable("configVar is unstable.")
config var configVar = "default";

@unstable("configConst is unstable.")
config var configConst = "default";

@unstable("configParam is unstable.")
config var configParam = "default";

writeln(privateConfigVarDepr);
writeln(configVarDepr);
writeln(configVar);
writeln(configConst);
writeln(configParam);

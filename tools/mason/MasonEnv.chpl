
use MasonUtils;
use MasonHelp;

proc MASON_HOME : string {
  const envHome = getEnv("MASON_HOME");
  const default = getEnv('HOME') + "/.mason";

  const masonHome = if envHome != "" then envHome else default;

  return masonHome;
}

proc MASON_CACHED_REGISTRY : string {
  const env = getEnv("MASON_CACHED_REGISTRY");
  if env == "" {
    return MASON_HOME + '/registry';
  } else {
    return env;
  }
}

proc MASON_REGISTRY : string {
  const env = getEnv("MASON_REGISTRY");
  const default = "https://github.com/chapel-lang/mason-registry";

  if env == "" {
    return default;
  } else {
    return env;
  }
}

proc masonEnv(args) {
  if hasOptions(args, "-h", "--help") {
    masonEnvHelp();
    exit(0);
  }

  const debug = hasOptions(args, "--debug");

  proc printVar(name : string, in val : string) {
    if getEnv(name) != "" then
      val += " *";

    writeln(name, ": ", val);
  }

  printVar("MASON_HOME", MASON_HOME);
  printVar("MASON_REGISTRY", MASON_REGISTRY);

  if debug {
    printVar("MASON_CACHED_REGISTRY", MASON_CACHED_REGISTRY);
  }
}

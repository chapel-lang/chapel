
use MasonModify;
use MasonUtils;
use TOML;
use FileSystem;

config const tf: string;
config const add: bool;
config const external: bool;
config const system: bool;

proc main() {

  // Add mason dependency
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd, tf);
  const result = modifyToml(add, "test@1.2.3", external, system, projectHome, tf=tf);
  writeln(result[1]);

  delete result(1);
}

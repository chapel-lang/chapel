
use MasonModify;
use MasonUtils;
use TOML;
use FileSystem;

config const tf: string;

proc main() {

  // Add mason dependency
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd, tf);
  const result = modifyToml(true, "test@1.2.3", true, false, projectHome, tf=tf);
  writeln(result[1]);

}

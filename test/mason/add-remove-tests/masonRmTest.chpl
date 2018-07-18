
use MasonModify;
use MasonUtils;
use TOML;
use FileSystem;

config const tf: string;

proc main() {

  // Add mason dependency
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd, tf);
  const result = modifyToml(false, "test@1.2.3", false, false, projectHome, tf=tf);
  writeln(result[1]);

}

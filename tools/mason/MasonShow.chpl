
use TOML;
use MasonUtils;


proc masonShow(args) throws {
  try! {
    if args.size != 2 {
      masonShowHelp();
      exit(0);
    }
    else {
      const cwd = getEnv("PWD");
      const projectHome = getProjectHome(cwd, "Mason.toml");
      const tomlPath = projectHome + "/Mason.toml";
      const openFile = openreader(tomlPath);
      const toml = parseToml(openFile);
      writeln(toml);
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}
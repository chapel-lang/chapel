use TOML;
use Path;
use Spawn;
use MasonEnv;
use MasonNew;
use MasonBuild;
use MasonHelp;
use MasonUtils;
use MasonExample;
use MasonModify;
use FileSystem;
use MasonTest;
private use List;
private use Map;

var files : list(string);
var dirs : list(string);

proc masonBench(args) throws {
  var show: bool = false;
  var skipUpdate = MASON_OFFLINE;
  var compopts: list(string);
  var countArgs = args.indices.low+2;

  for arg in args[args.indices.low+2..] {
    countArgs += 1;
    select(arg) {
      when '-h' {
        masonBenchHelp();
        exit(0);
      }
      when '--help' {
        masonBenchHelp();
        exit(0);
      }
      when '--show' {
        show = true;
      }
      otherwise {
        try! {
          // bench filenames, directories, compopts passed as args
          if isFile(arg) && arg.endsWith(".chpl") then files.append(arg);
          else if isDir(arg) then dirs.append(arg);
          else {
            if arg.startsWith('-') then compopts.append(arg);
          } 
        }
      }
    }
  }
  if show then printMetadata(compopts);
  try! {
    const cwd = getEnv("PWD"); 
    const projectHome = getProjectHome(cwd);
    var benchTestName: list(string);
    var benchTestPath = joinPath(projectHome, "bench");
    var benchTests = findfiles(startdir=benchTestPath, recursive=true, hidden=false);
    for bench in benchTests {
      if bench.endsWith(".chpl") {
        benchTestName.append(getBenchTestPath(bench));
      }
    }
    for bench in benchTestName do writeln(bench);
    updateLock(skipUpdate);
    runBenchTests(show, compopts);
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

proc runBenchTests(show, compopts) {
  for c in compopts do writeln(c); 
}

proc printMetadata(compopts: list(string)) {
  writeln("Files : ");
  for a in files do writeln(a);
  writeln("Dirs : ");
  for d in dirs do writeln(d);
  writeln("Compopts : ");
  for c in compopts do writeln(c);
}

// TODO: refactor getTestPath to accomodate bench tests and unit tests
proc getBenchTestPath(fullPath: string, testPath = "") : string {
  var split = splitPath(fullPath);
	writeln(split);
	return fullPath;
/*
  if split[1] == "bench" {
    return testPath;
  }
  else {
    if testPath == "" {
      return getTestPath(split[0], split[1]);
    }
    else {
      var appendedPath = joinPath(split[1], testPath);
      return getTestPath(split[0], appendedPath);
    }
  }
*/
}


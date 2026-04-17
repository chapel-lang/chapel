use List, Set;
use OS.POSIX, Path, FileSystem;
use Sort;
use IO;
use CTypes;

config const verbosePrint = false;
config const pythonToUse = "python3";
config const testName: string = "default";

extern proc setenv(name: c_ptrConst(c_char),
                   envval: c_ptrConst(c_char), overwrite: c_int): c_int;

proc cleanPath(in p) {
  p = p.strip();
  if p == "" then return "";
  if !exists(p) then return "";
  return realPath(p);
}

proc getSysPathSpawn() {
  use Subprocess;
  var p = spawn(
    [pythonToUse,
     "-c", "import sys; import site; site.main(); print(*sys.path, sep=',')"],
    stdout=pipeStyle.pipe);

  var output = p.stdout.readAll(string).strip();
  var paths = new list(string);
  for p in output.split(",") {
    var pp = cleanPath(p);
    if pp != "" && !paths.contains(pp) {
      paths.pushBack(pp);
    }
  }
  // add '.' to paths
  var dot = cleanPath(".");
  if dot != "" && !paths.contains(dot) {
    paths.pushBack(dot);
  }
  return paths;
}
proc getSysPathInterpreter() {
  use Python;
  var paths = new list(string);
  {
    var interp = new Interpreter();
    var sys = interp.importModule("sys");
    for p in sys.get(list(string), "path") {
      var pp = cleanPath(p);
      if pp != "" && !paths.contains(pp) {
        paths.pushBack(pp);
      }
    }
  }

  return paths;
}

proc comparePaths(paths1, paths2) {
  // sort the paths and compare
  var sortedPaths1 = sorted(paths1);
  var sortedPaths2 = sorted(paths2);

  if sortedPaths1.size != sortedPaths2.size {
    writeln("Mismatch in path sizes:", sortedPaths1.size, "vs",
            sortedPaths2.size, sep=" ");
    writeln("A: ", sortedPaths1);
    writeln("B: ", sortedPaths2);
    return false;
  }

  for (p1, p2) in zip(sortedPaths1, sortedPaths2) {
    if p1 != p2 {
      writeln("Mismatch in paths:", p1, "!=", p2, sep=" ");
      return false;
    }
  }
  return true;
}


proc default() {
  var pathsSpawn = getSysPathSpawn();
  var pathsInterpreter = getSysPathInterpreter();
  return (pathsSpawn, pathsInterpreter);
}
proc parentUserBase() {
  // add the parent directory of this file as the user base
  setenv("PYTHONUSERBASE", joinPath(try! here.cwd(), "..").c_str(), 1);
  setenv("PYTHONPATH", "", 1);

  var pathsSpawn = getSysPathSpawn();
  var pathsInterpreter = getSysPathInterpreter();
  return (pathsSpawn, pathsInterpreter);
}
proc parentPythonPath() {
  // add the parent directory of this file
  setenv("PYTHONPATH", joinPath(try! here.cwd(), "..").c_str(), 1);

  var pathsSpawn = getSysPathSpawn();
  var pathsInterpreter = getSysPathInterpreter();
  return (pathsSpawn, pathsInterpreter);
}
proc main() {
  var pathsSpawn, pathsInterpreter: list(string);
  if testName == "default" {
    (pathsSpawn, pathsInterpreter) = default();
  } else if testName == "parentUserBase" {
    (pathsSpawn, pathsInterpreter) = parentUserBase();
  } else if testName == "parentPythonPath" {
    (pathsSpawn, pathsInterpreter) = parentPythonPath();
  } else {
    writeln("Unknown test name: ", testName);
    return;
  }

  if verbosePrint {
    writeln("Running test for ", testName);
  }

  if verbosePrint {
    writeln("Spawn paths:");
    for p in sorted(pathsSpawn) {
      writeln("  ", p);
    }
    writeln("Interpreter paths:");
    for p in sorted(pathsInterpreter) {
      writeln("  ", p);
    }
  }

  if !comparePaths(pathsSpawn, pathsInterpreter) {
    writeln("Paths do not match for ", testName);
  }

}

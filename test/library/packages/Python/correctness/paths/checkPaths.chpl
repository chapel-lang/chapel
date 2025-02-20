use Map, List, Set;
use OS.POSIX, Path, FileSystem;
use Sort;
use IO;
use CTypes;

config const verbosePrint = false;
config const pythonToUse = "python3";

require "checkPaths.h", "checkPaths.c";
extern proc getEnvironment(): c_ptr(c_ptr(c_char));
extern proc setenv(name: c_ptrConst(c_char),
                   envval: c_ptrConst(c_char), overwrite: c_int): c_int;

proc cleanPath(in p) {
  p = p.strip();
  if p == "" then return "";
  if !exists(p) then return "";
  return realPath(p);
}

proc getCurrentEnv() {
  var env = new map(string, string);
  var environ = getEnvironment();
  while environ.deref() != nil {
    var keyVal = string.createBorrowingBuffer(environ.deref()).split("=");
    env[keyVal[0]] = keyVal[1];
    environ += 1;
  }
  return env;
}

proc getSysPathSpawn(const ref env: map(string, string) =
                      new map(string, string)) {
  var myEnv = getCurrentEnv();
  for k in env.keys() {
    myEnv[k] = env[k];
  }
  var envString = [k in env.keys()] "%s=%s".format(k, myEnv[k]);

  use Subprocess;
  var p = spawn(
    [pythonToUse, "-c", "import sys; print(*sys.path, sep=',')"],
    stdout=pipeStyle.pipe, env=envString);

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
proc getSysPathInterpreter(const ref env: map(string, string) =
                            new map(string, string)) {
  use Python;

  var oldEnv = new map(string, c_ptr(c_char));
  for k in env.keys() {
    var old = getenv(k.c_str());
    if old != nil then oldEnv[k] = old;
    setenv(k.c_str(), env[k].c_str(), 1);
  }

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

  for k in oldEnv.keys() {
    setenv(k.c_str(), oldEnv[k], 1);
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

proc testDriver(name, func) {
  if verbosePrint {
    writeln("Running test for ", name);
  }

  var (pathsSpawn, pathsInterpreter) = func();

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
    writeln("Paths do not match");
  }
}


proc default() {
  var pathsSpawn = getSysPathSpawn();
  var pathsInterpreter = getSysPathInterpreter();
  return (pathsSpawn, pathsInterpreter);
}
proc parentUserBase() {
  // add the parent directory of this file as the user base
  var env = new map(string, string);
  env["PYTHONUSERBASE"] = joinPath(try! here.cwd(), "..");

  var pathsSpawn = getSysPathSpawn(env);
  var pathsInterpreter = getSysPathInterpreter(env);
  return (pathsSpawn, pathsInterpreter);
}
proc parentPythonPath() {
  // add the parent directory of this file
  var env = new map(string, string);
  env["PYTHONPATH"] = joinPath(try! here.cwd(), "..");

  var pathsSpawn = getSysPathSpawn(env);
  var pathsInterpreter = getSysPathInterpreter(env);
  return (pathsSpawn, pathsInterpreter);
}
proc main() {
  testDriver("default", default);
  testDriver("parentUserBase", parentUserBase);
  testDriver("parentPythonPath", parentPythonPath);
}

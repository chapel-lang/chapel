/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



/* A helper file of utilities for Mason */
use Spawn;
use FileSystem;
use TOML;
use Path;

/* Gets environment variables for spawn commands */
extern proc getenv(name : c_string) : c_string;
proc getEnv(name: string): string {
  var cname: c_string = name.c_str();
  var value = getenv(cname);
  return value:string;
}


class MasonError : Error {
  var msg:string;
  proc init(msg:string) {
    this.msg = msg;
  }
  proc message() {
    return msg;
  }
}


/* Creates the rest of the project structure */
proc makeTargetFiles(binLoc: string, projectHome: string) {

  const target = joinPath(projectHome, 'target');
  const srcBin = joinPath(target, binLoc);
  const test = joinPath(target, 'test');

  if !isDir(target) {
    mkdir(target);
  }
  if !isDir(srcBin) {
    mkdir(srcBin);
  }
  if !isDir(test) {
    mkdir(test);
  }
}


proc stripExt(toStrip: string, ext: string) : string {
  if toStrip.endsWith(ext) {
    var stripped = toStrip[..toStrip.size - ext.length];
    return stripped;
  }
  else {
    return toStrip;
  }
}


/* Uses the Spawn module to create a subprocess */
proc runCommand(cmd, quiet=false) : string {
  var ret : string;

  var splitCmd = cmd.split();
  var process = spawn(splitCmd, stdout=PIPE);
  process.wait();

  for line in process.stdout.lines() {
    ret += line;
    if quiet == false {
      write(line);
    }
  }

  return ret;
}


/* Same as runCommand but for situations where an
   exit status is needed */
proc runWithStatus(command, show=true): int {

  try {
    var cmd = command.split();
    var sub = spawn(cmd, stdout=PIPE);

    var line:string;
    if show {
      while sub.stdout.readline(line) do write(line);
    }
    sub.wait();
    return sub.exit_status;
  }
  catch {
    return -1;
  }
}

proc hasOptions(args : [] string, const opts : string ...) {
  var ret = false;

  for o in opts {
    const (found, idx) = args.find(o);
    if found {
      ret = true;
      break;
    }
  }

  return ret;
}

record VersionInfo {
  var major = -1, minor = -1, bug = 0;

  proc init() {
    major = -1;
    minor = -1;
    bug = 0;
  }

  proc init(other:VersionInfo) {
    this.major = other.major;
    this.minor = other.minor;
    this.bug   = other.bug;
  }

  proc init(maj : int, min : int, bug: int) {
    this.major = maj;
    this.minor = min;
    this.bug   = bug;
  }

  proc init(str:string) {
    const s : [1..3] string = str.split(".");
    assert(s.size == 3);

    major = s[1]:int;
    minor = s[2]:int;
    bug   = s[3]:int;
  }

  proc str() {
    return major + "." + minor + "." + bug;
  }

  proc cmp(other:VersionInfo) {
    const A = (major, minor, bug);
    const B = (other.major, other.minor, other.bug);
    for i in 1..3 {
      if A(i) > B(i) then return 1;
      else if A(i) < B(i) then return -1;
    }
    return 0;
  }
}

proc >=(a:VersionInfo, b:VersionInfo) : bool {
  return a.cmp(b) >= 0;
}
proc <=(a:VersionInfo, b:VersionInfo) : bool {
  return a.cmp(b) <= 0;
}
proc ==(a:VersionInfo, b:VersionInfo) : bool {
  return a.cmp(b) == 0;
}
proc >(a:VersionInfo, b:VersionInfo) : bool {
  return a.cmp(b) > 0;
}


private var chplVersionInfo = (-1, -1, -1, false);
/*
   Returns a tuple containing information about the `chpl --version`:
   (major, minor, bugFix, isMaster)
*/
proc getChapelVersionInfo() {
  use Regexp;

  if chplVersionInfo(1) == -1 {
    try {
      var ret : (int, int, int, bool);

      var process = spawn(["chpl", "--version"], stdout=PIPE);
      process.wait();

      var output : string;
      for line in process.stdout.lines() {
        output += line;
      }

      const semverPattern = "(\\d+\\.\\d+\\.\\d+)";
      var master  = compile(semverPattern + " pre-release (\\([a-z0-9]+\\))");
      var release = compile(semverPattern);

      var semver, sha : string;
      if master.search(output, semver, sha) {
        ret(4) = true;
      } else if release.search(output, semver) {
        ret(4) = false;
      } else {
        throw new MasonError("Failed to match output of 'chpl --version':\n" + output);
      }

      const split = semver.split(".");
      for param i in 1..3 do ret(i) = split(i):int;

      chplVersionInfo = ret;
    } catch e : Error {
      stderr.writeln("Error while getting Chapel version:");
      stderr.writeln(e.message());
      exit(1);
    }
  }

  return chplVersionInfo;
}

private var chplVersion = "";
proc getChapelVersionStr() {
  if chplVersion == "" {
    const version = getChapelVersionInfo();
    chplVersion = version(1) + "." + version(2) + "." + version(3);
  }
  return chplVersion;
}

proc gitC(newDir, command, quiet=false) {
  var ret : string;

  const oldDir = here.cwd();
  here.chdir(newDir);

  ret = runCommand(command, quiet);

  here.chdir(oldDir);

  return ret;
}

proc developerMode: bool {
  const env = getEnv("CHPL_DEVELOPER");
  return env != "";
}


proc getProjectHome(cwd: string, tomlName="Mason.toml") : string throws {
  const (dirname, basename) = splitPath(cwd);
  if dirname == '/' {
    throw new MasonError("Mason could not find your configuration file (Mason.toml)");
  }
  const tomlFile = joinPath(cwd, tomlName);
  if exists(tomlFile) {
    return cwd;
  }
  return getProjectHome(dirname, tomlName);
}


extern "struct stat" record chpl_stat {
  var st_mtime: off_t;
}

proc getLastModified(filename: string) : int {
  extern proc stat(filename: c_string, ref chpl_stat): c_int;

  var file_buf: chpl_stat;
  var file_path = filename.c_str();

  if (stat(file_path, file_buf) == 0) {
    return file_buf.st_mtime;
    }
  return -1;
}

proc projectModified(projectHome, projectName, binLocation) : bool {
  const binaryPath = joinPath(projectHome, "target", binLocation, projectName);

  if isFile(binaryPath) {
    for file in listdir(joinPath(projectHome, "src")) {
      var srcPath = joinPath(projectHome, "src", file);
      if getLastModified(srcPath) > getLastModified(binaryPath) {
        return true;
      }
    }
    return false;
  }
  return true;
}

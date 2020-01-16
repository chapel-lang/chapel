/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
private use List;
private use Map;

public use Spawn;
public use FileSystem;
public use TOML;
public use Path;
public use MasonEnv;


/* Gets environment variables for spawn commands */
extern proc getenv(name : c_string) : c_string;
proc getEnv(name: string): string {
  var cname: c_string = name.c_str();
  var value = getenv(cname);
  return createStringWithNewBuffer(value);
}


class MasonError : Error {
  var msg:string;
  proc init(msg:string) {
    this.msg = msg;
  }
  override proc message() {
    return msg;
  }
}


/* Creates the rest of the project structure */
proc makeTargetFiles(binLoc: string, projectHome: string) {

  const target = joinPath(projectHome, 'target');
  const srcBin = joinPath(target, binLoc);
  const example = joinPath(target, 'example');

  if !isDir(target) {
    mkdir(target);
  }
  if !isDir(srcBin) {
    mkdir(srcBin);
  }
  if !isDir(example) {
    mkdir(example);
  }

  const actualTest = joinPath(projectHome,'test');
  if isDir(actualTest) {
    for dir in walkdirs(actualTest) {
      const internalDir = target+dir.replace(projectHome,"");
      if !isDir(internalDir) {
        mkdir(internalDir);
      }
    }
  }
  const test = joinPath(target, 'test');
  if(!isDir(test)) {
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
proc runCommand(cmd, quiet=false) : string throws {
  var ret : string;
  try {

    var splitCmd = cmd.split();
    var process = spawn(splitCmd, stdout=PIPE);

    for line in process.stdout.lines() {
      ret += line;
      if quiet == false {
        write(line);
      }
    }
    process.wait();
  }
  catch {
    throw new owned MasonError("Internal mason error");
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

proc runWithProcess(command, quiet=false) throws {
  try {
    var cmd = command.split();
    var process = spawn(cmd, stdout=PIPE, stderr=PIPE);

    return process;
  }
  catch {
    throw new owned MasonError("Internal mason error");
    exit(0);
  }
}

proc SPACK_ROOT : string {
  const envHome = getEnv("SPACK_ROOT");
  const default = MASON_HOME + "/spack";

  const spackRoot = if !envHome.isEmpty() then envHome else default;

  return spackRoot;
}

/* uses spawnshell and the prefix to setup Spack before
   calling the spack command. This also returns the stdout
   of the spack call.
   TODO: get to work with Spawn */
proc getSpackResult(cmd, quiet=false) : string throws {
  var ret : string;
  try {


    var prefix = "export SPACK_ROOT=" + SPACK_ROOT +
    " && export PATH=\"$SPACK_ROOT/bin:$PATH\"" +
    " && . $SPACK_ROOT/share/spack/setup-env.sh && ";
    var splitCmd = prefix + cmd;
    var process = spawnshell(splitCmd, stdout=PIPE, executable="bash");

    for line in process.stdout.lines() {
      ret += line;
      if quiet == false {
        write(line);
      }
    }
    process.wait();
  }
  catch {
    throw new owned MasonError("Internal mason error");
  }
  return ret;
}


/* Sets up spack by prefixing command with spack env vars
   Only returns the exit status of the command
   TODO: get to work with Spawn */
proc runSpackCommand(command) {

  var prefix = "export SPACK_ROOT=" + SPACK_ROOT +
    " && export PATH=\"$SPACK_ROOT/bin:$PATH\"" +
    " && . $SPACK_ROOT/share/spack/setup-env.sh && ";

  var cmd = (prefix + command);
  var sub = spawnshell(cmd, stderr=PIPE, executable="bash");
  sub.wait();

  for line in sub.stderr.lines() {
    write(line);
  }

  return sub.exit_status;
}


proc hasOptions(args: list(string), const opts: string ...) {
  var ret = false;

  for o in opts {
    const found = args.count(o) != 0;
    if found {
      ret = true;
      break;
    }
  }

  return ret;
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

  proc init=(other:VersionInfo) {
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
    return major:string + "." + minor:string + "." + bug:string;
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

  proc this(i: int): int {
    select i {
      when 1 do
        return this.major;
      when 2 do
        return this.minor;
      when 3 do
        return this.bug;
      otherwise
        halt('Out of bounds access of VersionInfo');
    }
  }

  proc containsMax() {
    return this.major == max(int) || this.minor == max(int) || this.bug == max(int);
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

proc <(a:VersionInfo, b:VersionInfo) : bool {
  return a.cmp(b) < 0;
}


private var chplVersionInfo = new VersionInfo(-1, -1, -1);
/*
   Returns a tuple containing information about the `chpl --version`:
   (major, minor, bugFix, isMaster)
*/
proc getChapelVersionInfo(): VersionInfo {
  use Regexp;

  if chplVersionInfo(1) == -1 {
    try {

      var ret : VersionInfo;

      var process = spawn(["chpl", "--version"], stdout=PIPE);
      process.wait();
      if process.exit_status != 0 {
        throw new owned MasonError("Failed to run 'chpl --version'");
      }


      var output : string;
      for line in process.stdout.lines() {
        output += line;
      }

      const semverPattern = "(\\d+\\.\\d+\\.\\d+)";
      var master  = compile(semverPattern + " pre-release (\\([a-z0-9]+\\))");
      var release = compile(semverPattern);

      var semver, sha : string;
      var isMaster: bool;
      if master.search(output, semver, sha) {
        isMaster = true;
      } else if release.search(output, semver) {
        isMaster = false;
      } else {
        throw new owned MasonError("Failed to match output of 'chpl --version':\n" + output);
      }

      const split = semver.split(".");
      chplVersionInfo = new VersionInfo(split[1]:int, split[2]:int, split[3]:int);
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
    chplVersion = version(1):string + "." + version(2):string + "." + version(3):string;
  }
  return chplVersion;
}

proc gitC(newDir, command, quiet=false) throws {
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
    throw new owned MasonError("Mason could not find your configuration file (Mason.toml)");
  }
  const tomlFile = joinPath(cwd, tomlName);
  if exists(tomlFile) {
    return cwd;
  }
  return getProjectHome(dirname, tomlName);
}


extern "struct sys_stat_s" record chpl_stat {
  var st_mtim: chpl_timespec;
}

extern "struct timespec" record chpl_timespec {
  var tv_sec: int;
  var tv_nsec: int;
}

proc getLastModified(filename: string) : int {
  use SysCTypes;

  extern proc sys_stat(filename: c_string, ref chpl_stat): c_int;

  var file_buf: chpl_stat;
  var file_path = filename.c_str();

  if (sys_stat(file_path, file_buf) == 0) {
    return file_buf.st_mtim.tv_sec;
    }
  return -1;
}

proc projectModified(projectHome, projectName, binLocation) : bool {
  const binaryPath = joinPath(projectHome, "target", binLocation, projectName);
  const tomlPath = joinPath(projectHome, "Mason.toml");

  if isFile(binaryPath) {
    const binModTime = getLastModified(binaryPath);
    for file in listdir(joinPath(projectHome, "src")) {
      var srcPath = joinPath(projectHome, "src", file);
      if getLastModified(srcPath) > binModTime {
        return true;
      }
    }
    if getLastModified(tomlPath) > binModTime {
      return true;
    }
    return false;
  }
  return true;
}

/* Return 'true' for valid identifiers according to Chapel parser/spec,
   otherwise 'false' */
proc isIdentifier(name:string) {

  // Identifiers can't be empty
  if name == "" then
    return false;

  // Identifiers can't start with a digit or a $
  if name[1].isDigit() then
    return false;
  if name[1] == "$" then
    return false;

  // Check all characters are legal identifier characters
  // - lower case alphabetic
  // - upper case alphabetic
  // - digits
  // - _
  // - $
  var ok = true;
  for ch in name {
    if !(ch == "$" || ch == "_" || ch.isAlnum()) then
      ok = false;
  }
  return ok;
}


/* Iterator to collect fields from a toml
   TODO custom fields returned */
iter allFields(tomlTbl: unmanaged Toml) {
  for (k,v) in tomlTbl.A.items() {
    if v.tag == fieldtag.fieldToml then
      continue;
    else yield(k,v);
  }
}

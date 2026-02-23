/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
private use CTypes;
private use ChplConfig;
public use FileSystem;
private use List;
private use Map;
public use Subprocess;
public use MasonEnv;
public use Path;
public use TOML;
use Regex;
import MasonLogger;

private var log = new MasonLogger.logger("mason utils");


/* Gets environment variables for spawn commands */
proc getEnv(name: string): string {
  extern proc getenv(name : c_ptrConst(c_char)) : c_ptrConst(c_char);
  var cname = name.c_str();
  var value = getenv(cname);
  return string.createCopyingBuffer(value);
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
    for dir in walkDirs(actualTest) {
      const internalDir = target+dir.replace(projectHome,"");
      if !isDir(internalDir) {
        mkdir(internalDir);
      }
    }
  }
  const test = joinPath(target, 'test');
  if !isDir(test) {
    mkdir(test);
  }
}


proc stripExt(toStrip: string, ext: string) : string {
  if toStrip.endsWith(ext) {
    var stripped = toStrip[..<(toStrip.size - ext.size)];
    return stripped;
  } else {
    return toStrip;
  }
}


/* Uses the Subprocess module to create a subprocess */
proc runCommand(cmd: [] string, quiet=false) : string throws {
  var ret : string;
  try {
    log.debugf("runCommand: %?\n", cmd);
    var process = spawn(cmd, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);


    log.debugln("stdout:");
    // use .lines() to avoid https://github.com/chapel-lang/chapel/issues/28211
    for line in process.stdout.lines() {
      ret += line;
      if quiet then log.debug(line); else log.info(line);
    }
    log.debugln("end stdout");

    log.debugln("stderr:");
    for line in process.stderr.lines() {
      log.warn(line);
    }
    log.debugln("end stderr.");

    process.wait();

    log.debugf("exitCode: %i\n", process.exitCode);
    if process.exitCode != 0 {
      var cmdStr = " ".join(cmd);
      throw new owned MasonError("Command failed: '" + cmdStr + "'");
    }
  } catch e: FileNotFoundError {
    log.debugf("Caught FileNotFoundError for command: %?\n", cmd);
    var cmdStr = " ".join(cmd);
    throw new owned MasonError("Command not found: '" + cmdStr + "'");
  } catch e: MasonError {
    throw e;
  } catch e {
    log.debugf("Caught unknown error ('%?') for command: %?\n", e, cmd);
    throw new owned MasonError("Internal mason error");
  }
  return ret;
}
proc runCommand(cmd: string, quiet=false) : string throws {
  // temporary is a workaround for #27504
  const cmds = cmd.split();
  return runCommand(cmds, quiet=quiet);
}

/* Same as runCommand but for situations where an
   exit status is needed */
proc runWithStatus(command: string, quiet=false): int {
  var cmd = command.split();
  return runWithStatus(cmd, quiet=quiet);
}
proc runWithStatus(command: [] string, quiet=false): int {
  try {
    log.debugf("runWithStatus: %?\n", command);
    var sub = spawn(command, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);

    var line:string;
    if !quiet {
      while sub.stdout.readLine(line) do write(line);
      while sub.stderr.readLine(line) do write(line);
    }
    sub.wait();
    return sub.exitCode;
  } catch e {
    log.debugf("Caught unknown error ('%?') for command: %?\n", e, command);
    return -1;
  }
}

@chplcheck.ignore("CamelCaseFunctions")
proc SPACK_ROOT : string {
  const envHome = getEnv("SPACK_ROOT");
  const default = MASON_HOME + "/spack";

  const spackRoot = if !envHome.isEmpty() then envHome else default;

  return spackRoot;
}

/*
  Returns the current CHPL_HOME. Tries the following in order:
  1. The CHPL_HOME environment variable
  2. Using the `chpl` in PATH to print CHPL_HOME
  3. The CHPL_HOME of the chpl that built this mason (`ChplConfig.CHPL_HOME`)
*/
@chplcheck.ignore("CamelCaseFunctions")
proc CHPL_HOME : string {

  @chplcheck.ignore("CamelCaseFunctions")
  proc CHPL_HOME_inner() : string {
    proc getChplHomeFromChpl(): string {
      var chplHome = "";
      try {
        var process = spawn(["chpl", "--print-chpl-home"],
                             stdout=pipeStyle.pipe);
        for line in process.stdout.lines() {
          chplHome = line.strip();
        }
      } catch {
        chplHome = "";
      }
      return chplHome;
    }

    const env = getEnv("CHPL_HOME");
    const chplHome = if !env.isEmpty() then env else getChplHomeFromChpl();
    return if !chplHome.isEmpty() then chplHome else ChplConfig.CHPL_HOME;
  }

  @functionStatic
  ref chplHome = CHPL_HOME_inner();
  return chplHome;
}


/*
This fetches the mason-installed spack registry only.
Users that define SPACK_ROOT to their own spack installation will use
the registry of their spack installation.
*/
proc getSpackRegistry : string {
  return MASON_HOME + "/spack-registry";
}

/* uses spawnshell and the prefix to setup Spack before
   calling the spack command. This also returns the stdout
   of the spack call.
   TODO: get to work with Subprocess */
proc getSpackResult(cmd, quiet=false) : string throws {
  var ret : string;
  var prefix = "export SPACK_ROOT=" + SPACK_ROOT +
               " && export PATH=\"$SPACK_ROOT/bin:$PATH\"" +
               " && . $SPACK_ROOT/share/spack/setup-env.sh && ";
  var splitCmd = prefix + cmd;
  try {
    log.debugf("running spack command %s\n", splitCmd);
    var process = spawnshell(splitCmd,
                             stdout=pipeStyle.pipe, executable="bash");

    for line in process.stdout.lines() {
      ret += line;
      if !quiet {
        write(line);
      }
    }
    process.wait();
  } catch e {
    log.debugf("Caught unknown error ('%?') for command: %?\n", e, splitCmd);
    throw new owned MasonError("Internal mason error");
  }
  return ret;
}


/* Sets up spack by prefixing command with spack env vars
   Only returns the exit status of the command
   TODO: get to work with Subprocess */
proc runSpackCommand(command, quiet=false) {

  var prefix = "export SPACK_ROOT=" + SPACK_ROOT +
    " && export PATH=\"$SPACK_ROOT/bin:$PATH\"" +
    " && . $SPACK_ROOT/share/spack/setup-env.sh && ";

  var cmd = (prefix + command);
  var sub = spawnshell(cmd, stdout=pipeStyle.pipe,
                            stderr=pipeStyle.pipe, executable="bash");

  // quiet flag necessary for tests to be portable
  if !quiet {
    var line:string;
    while sub.stdout.readLine(line) {
      write(line);
    }
  }
  sub.wait();

  for line in sub.stderr.lines() {
    write(line);
  }

  return sub.exitCode;
}

// TODO: Can we get away with the Chapel Version object instead?
record versionInfo {
  var major = -1, minor = -1, bug = 0;

  proc init() {
    major = -1;
    minor = -1;
    bug = 0;
  }

  proc init=(other: versionInfo) {
    this.major = other.major;
    this.minor = other.minor;
    this.bug   = other.bug;
  }

  proc init(maj: int, min: int, bug: int) {
    this.major = maj;
    this.minor = min;
    this.bug   = bug;
  }

  proc init(str: string) {
    const s: [1..3] string = str.split(".");
    assert(s.size == 3);

    major = s[1]:int;
    minor = s[2]:int;
    bug   = s[3]:int;
  }

  proc str() {
    return major:string + "." + minor:string + "." + bug:string;
  }

  proc cmp(other: versionInfo) {
    const A = (major, minor, bug);
    const B = (other.major, other.minor, other.bug);
    for i in 0..2 {
      if A(i) > B(i) then return 1;
      else if A(i) < B(i) then return -1;
    }
    return 0;
  }

  proc this(i: int): int {
    select i {
      when 0 do
        return this.major;
      when 1 do
        return this.minor;
      when 2 do
        return this.bug;
      otherwise
        halt('Out of bounds access of versionInfo');
    }
  }

  proc containsMax() {
    return this.major == max(int) ||
           this.minor == max(int) ||
           this.bug == max(int);
  }

  proc isCompatible(other: versionInfo) : bool {
    // checks that a version is compatible with this version
    // versions are assumed compatible if major and minor versions match
    // and patch/bug level is the same or greater
    return this.major == other.major
           && this.minor == other.minor
           && this.bug <= other.bug;
  }

  proc type zero(): versionInfo {
    return new versionInfo(0, 0, 0);
  }
}

operator versionInfo.=(ref lhs: versionInfo, const ref rhs: versionInfo) {
  lhs.major = rhs.major;
  lhs.minor = rhs.minor;
  lhs.bug   = rhs.bug;
}

operator versionInfo.>=(a: versionInfo, b: versionInfo) : bool {
  return a.cmp(b) >= 0;
}
operator versionInfo.<=(a: versionInfo, b: versionInfo) : bool {
  return a.cmp(b) <= 0;
}
operator ==(a: versionInfo, b: versionInfo) : bool {
  return a.cmp(b) == 0;
}
operator versionInfo.>(a: versionInfo, b: versionInfo) : bool {
  return a.cmp(b) > 0;
}

operator versionInfo.<(a: versionInfo, b: versionInfo) : bool {
  return a.cmp(b) < 0;
}


private var chplVersionInfo = new versionInfo(-1, -1, -1);
/*
   Returns a tuple containing information about the `chpl --version`:
   (major, minor, bugFix, isMain)
*/
proc getChapelVersionInfo(): versionInfo throws {
  use Regex;

  if chplVersionInfo(0) == -1 {

    var output : string;
    try {
      output = runCommand(["chpl", "--version"], quiet=true);
    } catch {
      throw new MasonError("Failed to run 'chpl --version'");
    }

    const semverPattern = "(\\d+\\.\\d+\\.\\d+)";
    var main  = new regex(semverPattern + " pre-release (\\([a-z0-9]+\\))");
    var release = new regex(semverPattern);

    var semver, sha : string;
    var isMain: bool;
    if main.search(output, semver, sha) {
      isMain = true;
    } else if release.search(output, semver) {
      isMain = false;
    } else {
      throw new MasonError("Failed to match output of 'chpl --version':\n" +
                            output);
    }

    const split = semver.split(".");
    chplVersionInfo = new versionInfo(split[0]:int, split[1]:int, split[2]:int);
  }

  return chplVersionInfo;
}

private var chplVersion = "";
proc getChapelVersionStr() throws {
  if chplVersion == "" {
    const version = getChapelVersionInfo();
    chplVersion = version(0):string + "." +
                  version(1):string + "." +
                  version(2):string;
  }
  return chplVersion;
}

proc gitC(newDir, command, quiet=false) throws {
  var ret : string;
  const oldDir = here.cwd();
  here.chdir(newDir);
  defer here.chdir(oldDir);
  ret = runCommand(command, quiet);

  return ret;
}

proc developerMode: bool {
  const env = getEnv("CHPL_DEVELOPER");
  return env != "";
}


proc getProjectHome(cwd: string, tomlName="Mason.toml") : string throws {
  const (dirname, basename) = splitPath(cwd);
  if dirname == '/' {
    throw new MasonError("Mason could not find your " +
                         "configuration file (Mason.toml)");
  }
  const tomlFile = joinPath(cwd, tomlName);
  if exists(tomlFile) {
    return cwd;
  }
  return getProjectHome(dirname, tomlName);
}

proc getLastModified(filename: string) : int {
  use CTypes, OS.POSIX;

  var file_buf: struct_stat;
  var file_path = filename.c_str();

  if stat(file_path, c_ptrTo(file_buf)) == 0 then
    return file_buf.st_mtim.tv_sec;
  else
    return -1;
}

proc projectModified(projectHome, projectName, binLocation) : bool {
  const binaryPath = joinPath(projectHome, "target", binLocation, projectName);
  const tomlPath = joinPath(projectHome, "Mason.toml");

  if isFile(binaryPath) {
    const binModTime = getLastModified(binaryPath);
    for file in findFiles(joinPath(projectHome, "src"), recursive=true) {
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

  // Identifiers can't start with a digit
  if name[0].isDigit() then
    return false;

  // Check all characters are legal identifier characters
  // - lower case alphabetic
  // - upper case alphabetic
  // - digits
  // - _
  var ok = true;
  for ch in name {
    if !(ch == "_" || ch.isAlnum()) then
      ok = false;
  }
  return ok;
}


record gitSource {
  var url: string;
  var name: string;
  var branch: string;
  var revision: string;

  iter type iterList(x: list(gitSource)) {
    for item in x {
      yield (item.url, item.name, item.branch, item.revision);
    }
  }
  iter type iterList(x: list(gitSource), param tag: iterKind)
  where tag == iterKind.standalone {
    foreach idx in 0..#x.size {
      const item = x[idx];
      yield (item.url, item.name, item.branch, item.revision);
    }
  }
}
record srcSource {
  var url: string;
  var name: string;
  var version: string;

  iter type iterList(x: list(srcSource)) {
    for item in x {
      yield (item.url, item.name, item.version);
    }
  }
  iter type iterList(x: list(srcSource), param tag: iterKind)
  where tag == iterKind.standalone {
    foreach idx in 0..#x.size {
      const item = x[idx];
      yield (item.url, item.name, item.version);
    }
  }
}

proc getMasonDependencies(sourceList: list(srcSource),
                          gitList: list(gitSource),
                          progName: string): list(string) {
  // Declare example to run as the main module
  var masonCompopts = new list(string);
  masonCompopts.pushBack("--main-module");
  masonCompopts.pushBack(progName);

  if sourceList.size > 0 {
    const depPath = joinPath(MASON_HOME, "src");

    // Add dependencies to project
    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, version) in srcSource.iterList(sourceList) {
      const depSrc = joinPath(depPath, "%s-%s".format(name, version),
                              "src", "%s.chpl".format(name));
      masonCompopts.pushBack(depSrc);
    }
  }
  if gitList.size > 0 {
    const gitDepPath = joinPath(MASON_HOME, "git");

    // Add git dependencies
    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, branch, _y) in gitSource.iterList(gitList) {
      const gitDepSrc = joinPath(gitDepPath, "%s-%s".format(name, branch),
                                 "src", "%s.chpl".format(name));
      masonCompopts.pushBack(gitDepSrc);
    }
  }
  return masonCompopts;
}

/* Checks to see if dependency has already been
   downloaded previously */
proc depExists(dependency: string, repo='/src/') {
  var repos = MASON_HOME + repo;
  var exists = false;
  for dir in listDir(repos) {
    if dir == dependency then
      exists = true;
  }
  return exists;
}


proc getProjectType(): string throws {
  const cwd = here.cwd();
  const projectHome = getProjectHome(cwd);
  const toParse = open(projectHome + "/Mason.toml", ioMode.r);
  const tomlFile = parseToml(toParse);
  if const type_ = tomlFile.get("brick.type") then
    return type_.s;
  throw new MasonError('Type not found in TOML file; '+
                       'please add a type="application" key');
}

record package {
  var name: string;
  var version: versionInfo;
  var registry: string;

  proc brickPath() {
    const tomlName = version.str() + ".toml";
    return joinPath(registry, "Bricks", name, tomlName);
  }

  proc type nullPackage() {
    return new package("", versionInfo.zero(), "");
  }

  operator <(a: package, b: package) : bool {
    if a.name < b.name then
      return true;
    else if a.name.toLower() == b.name.toLower() then
      return a.version < b.version;
    else
      return false;
  }
}

use Sort;
record pkgComparator: relativeComparator {
  var query: string;
  proc compare(a: package, b: package) {
    if query != "" {
      if a.name.toLower().startsWith(query.toLower()) &&
         !b.name.toLower().startsWith(query.toLower()) then
        return -1;
      else if !a.name.toLower().startsWith(query.toLower()) &&
              b.name.toLower().startsWith(query.toLower()) then
        return 1;
    }

    if a < b then
      return -1;
    else if b < a then
      return 1;
    else
      return 0;
  }
}

proc isHidden(name: string) : bool {
  return name.startsWith("_");
}
proc searchDependencies(pattern: regex(string)): list(package) throws {
  var pkgs: list(package);
  for registry in MASON_CACHED_REGISTRY {
    const searchDir = joinPath(registry, "Bricks");
    if !isDir(searchDir) then
      throw new MasonError("Registry path '" + registry +
                           "' does not exist.\n" +
                           "Try running 'mason update --force' or "+
                           "'mason search --update'.");

    for dir in listDir(searchDir, files=false, dirs=true) {
      const name = dir.replace("/", "");
      if pattern.search(name) {
        if isHidden(name) {
          log.debugln("found hidden package: " + name);
        } else {
          const ver = findLatest(joinPath(searchDir, dir));
          if ver != versionInfo.zero() {
            pkgs.pushBack(new package(name, ver, registry));
          }
        }
      }
    }
  }
  return pkgs;
}

/* Return parsed TOML file of name depName if it is located
   in a registry in MASON_CACHED_REGISTRY. If dependency is
   not found, throw an error. */
proc getDepToml(depName: string, depVersion: string) throws {
  const pattern = new regex(depName, ignoreCase=true);

  var pkgs = searchDependencies(pattern);

  var foundDep = package.nullPackage();
  for pkg in pkgs {
    if pkg.name == depName && pkg.version.str() == depVersion {
      foundDep = pkg;
      break;
    }
  }

  if foundDep.name == "" {
    throw new MasonError("No TOML file in registry for " +
                         depName + "@" + depVersion);
  }

  const brickPath = foundDep.brickPath();
  const openFile = openReader(brickPath, locking=false);
  const toml = parseToml(openFile);
  return toml;

}


/* Search TOML files within a package directory to find the latest package
   version number that is supported with current Chapel version */
proc findLatest(packageDir: string): versionInfo {
  use Path;

  var ret = versionInfo.zero();
  const suffix = ".toml";
  const packageName = basename(packageDir);
  for manifest in listDir(packageDir, files=true, dirs=false) {
    // Check that it is a valid TOML file
    if !manifest.endsWith(suffix) {
      log.warnf("File without '.toml' extension encountered - skipping %s %s\n",
                packageName, manifest);
      continue;
    }

    // Skip packages that are out of version bounds
    const chplVersion = getChapelVersionInfo();

    const manifestReader = openReader(joinPath(packageDir, manifest),
                                      locking=false);
    const manifestToml = parseToml(manifestReader);
    const brick = manifestToml['brick'];
    var (low, high) = parseChplVersion(brick);
    if chplVersion < low || chplVersion > high then continue;

    // Check that Chapel version is supported
    const end = manifest.size - suffix.size;
    const ver = new versionInfo(manifest[0..<end]);
    if ver > ret then ret = ver;
  }
  return ret;
}

/* Reads the Chapel version specified by a mason project's
   TOML file and returns the min and max compatible versions */
proc parseChplVersion(brick: borrowed Toml?): (versionInfo, versionInfo) {
  use Regex;

  if brick == nil {
    stderr.writeln("Error: Unable to parse manifest file");
    exit(1);
  }

  // Assert some expected fields are not nil
  if brick!.get['name'] == nil || brick!.get['version'] == nil {
    stderr.writeln("Error: Unable to parse manifest file");
    exit(1);
  }

  if brick!.get['chplVersion'] == nil {
    const name = brick!["name"]!.s + "-" + brick!["version"]!.s;
    stderr.writeln("Brick '", name, "' missing required 'chplVersion' field");
    exit(1);
  }

  const chplVersion = brick!["chplVersion"]!.s;
  var low, high: versionInfo;

  try {
    (low, high) = checkChplVersion(chplVersion);
  } catch e : Error {
    const name = brick!["name"]!.s + "-" + brick!["version"]!.s;
    stderr.writeln("Invalid chplVersion in package '",
                   name, "': ", chplVersion);
    stderr.writeln("Details: ", e.message());
    exit(1);
  }

  return (low, high);
}

/* Ensure that Chapel version is properly formatted. Returns
   a tuple of the low, high supported verisons.*/
proc checkChplVersion(chplVersion) throws {
  use Regex;
  var lo, hi : versionInfo;
  const formatMessage = "\n\n" +
    "chplVersion format must be '<version>..<version>' or '<version>'\n" +
    "A <version> must be in one of the following formats:\n" +
    "  x.x.x\n" +
    "  x.x\n" +
    "where 'x' is a positive integer.\n";

  var versions = chplVersion.split("..");
  [v in versions] v = v.strip();

  // Expecting 1 or 2 version strings
  if versions.size > 2 || versions.size < 1 {
    throw new MasonError("Expecting 1 or 2 versions in chplVersion range." +
                          formatMessage);
  } else if versions.size == 2 && (versions[0] == "" || versions[1] == "") {
    throw new MasonError("Unbounded chplVersion ranges are not allowed." +
                         formatMessage);
  }

  proc parseString(ver:string): versionInfo throws {
    var ret : versionInfo;

    // Finds 'x.x' or 'x.x.x' where x is a positive number
    const pattern = new regex("^(\\d+\\.\\d+(\\.\\d+)?)$");
    var semver : string;
    if !pattern.match(ver, semver).matched {
      throw new MasonError("Invalid Chapel version format: " + ver +
                            formatMessage);
    }
    const nums = for s in semver.split(".") do s:int;
    ret.major = nums[0];
    ret.minor = nums[1];
    if nums.size == 3 then ret.bug = nums[2];

    return ret;
  }

  lo = parseString(versions[0]);

  if versions.size == 1 {
    hi = new versionInfo(max(int), max(int), max(int));
  } else {
    hi = parseString(versions[1]);
  }
  if lo > hi then
    throw new MasonError("Lower bound of chplVersion must be <= upper bound: " +
                          lo.str() + " > " + hi.str());

  return (lo, hi);
}

/* Print a TOML file. Expects full path. */
proc showToml(tomlFile : string) {
  const openFile = openReader(tomlFile, locking=false);
  const toml = parseToml(openFile);
  writeln(toml);
  openFile.close();
}

/*
  Takes projectName, vcs (version control), show as inputs and
  initializes a library project at a directory of given projectName
  A library project consists of .gitignore file, Mason.toml file, and
  directories such as .git, src, example, test
*/
proc initProject(dirName, packageName, vcs, show,
                 version: string, chplVersion: string, license: string,
                 packageType: string) throws {
  if packageType == "light" {
    const path = if dirName == "" then here.cwd() else dirName;
    const lightName = if packageName == ""
                        then basename(here.cwd())
                        else packageName;
    mkdir(dirName);
    makeBasicToml(dirName=lightName, path=path, version, chplVersion,
                  license, packageType);
  } else {
    if vcs {
      gitInit(dirName, show);
      addGitIgnore(dirName);
    } else {
      mkdir(dirName);
    }
    // Confirm git init before creating files
    if isDir(dirName) {
      makeBasicToml(dirName=packageName, path=dirName, version, chplVersion,
                    license, packageType);
      makeSrcDir(dirName);
      makeModule(dirName, fileName=packageName, packageType);
    } else {
      throw new MasonError("Failed to create project");
    }
  }
  if packageName != "" then
    writeln("Created new " + packageType + " project: " + packageName);
  else
    writeln("Created new " + packageType + " project: " + basename(here.cwd()));
}

/* Iterator to collect fields from a toml
   TODO custom fields returned */
iter allFields(tomlTbl: Toml) {
  for (k,v) in zip(tomlTbl.A.keys(), tomlTbl.A.values()) {
    if v!.tag == fieldtag.fieldToml then
      continue;
    else yield(k,v);
  }
}

proc isStringOrStringArray(toml: Toml) : bool {
  if toml.tomlType == "string" {
    return true;
  } else if toml.tomlType == "array" {
    const tomlArr = toml.arr;
    for f in tomlArr {
      if f == nil || f!.tomlType != "string" {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}

proc parseCompilerOptions(toml: Toml): list(string) throws {
  var res = new list(string);
  if !isStringOrStringArray(toml) {
    throw new MasonError("unable to parse: "+
                         "expected a string or an array of strings");
  }

  if toml.tomlType == "string" {
    res.pushBack(toml.s.split(" "));
  } else {
    for f in toml.arr {
      res.pushBack(f!.s);
    }
  }
  return res;
}


record chplOptions {
  var compopts: list(string);
  var execopts: list(string);
}

@chplcheck.ignore("CamelCaseFunctions")
proc MASON_VERSION : string {
  return "0.2.0";
}

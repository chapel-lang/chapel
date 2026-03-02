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

import MasonPrereqs;

use ArgumentParser;
use List;
use Map;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonUpdate;
use MasonSystem;
use MasonExternal;
use MasonExample;
use MasonLogger;
use Subprocess;
use TOML;

import Path;
import MasonPrereqs;

private var log = new logger("mason build");

proc masonBuild(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonBuildHelpHandler());

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var forceFlag = parser.addFlag(name="force", defaultValue=false);
  var exampleOpts = parser.addOption(name="example", numArgs=0..);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);
  log.debugln("Arguments parsed");

  const projectType = getProjectType();
  if projectType == "light" then
    throw new MasonError("Mason light projects do not " +
                         "currently support 'mason build'");

  log.debugln("Project type acquired");

  var show = showFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var force = forceFlag.valueAsBool();
  var compopts: list(string);
  var example = exampleOpts._present; // --example provided w/wo a value
  var skipUpdate = MASON_OFFLINE;

  if updateFlag.hasValue() {
    if updateFlag.valueAsBool() then skipUpdate = false;
    else skipUpdate = true;
  }

  MasonPrereqs.install();

  log.debugf("Is example? %s\n", example);
  if example {
    var examples = new list(exampleOpts.values());
    var extraCompopts = new list(passArgs.values());
    runExamples(show=show, run=false, build=true, release=release,
                skipUpdate=skipUpdate, force=force,
                examplesRequested=examples,
                extraCompopts=extraCompopts, nLocales=1);
  } else {
    if passArgs.hasValue() {
      for val in passArgs.values() do compopts.pushBack(val);
    }
    const configNames = updateLock(skipUpdate);
    const tomlName = configNames[0];
    const lockName = configNames[1];
    log.debugln("About to build program");
    buildProgram(release, show, force, skipUpdate,
                 compopts, tomlName, lockName);
  }
}

private proc checkChplVersion(lockFile: borrowed Toml) throws {
  const root = lockFile["root"]!;
  const (success, low, hi) = verifyChapelVersion(root);

  if !success {
    throw new MasonError("Build failure: lock file expecting chplVersion " +
                         prettyVersionRange(low, hi));
  }
}


proc buildProgram(release: bool, show: bool, force: bool, skipUpdate: bool,
                  cmdLineCompopts: list(string), tomlName="Mason.toml",
                  lockName="Mason.lock") throws {
  const cwd = here.cwd();
  const projectHome = getProjectHome(cwd, tomlName);
  const lockPath = joinPath(projectHome, lockName);
  if !isFile(lockPath) then
    throw new owned MasonError("Cannot build: no Mason.lock found");

  const toParse = open(lockPath, ioMode.r);
  defer toParse.close();
  var lockFile = parseToml(toParse);
  const projectName = lockFile["root.name"]!.s;

  var binLoc = 'debug';
  if release then
    binLoc = 'release';


  // build on last modification
  const fingerprintDir =
    joinPath(projectHome, "target", binLoc, ".fingerprint");
  const fingerprintChanged =
    !checkFingerprint(projectName, fingerprintDir,
                      computeFingerprint(cmdLineCompopts));
  if force ||
     projectModified(projectHome, projectName, binLoc) || fingerprintChanged {

    // Make build files and check chapel version
    makeTargetFiles(binLoc, projectHome);
    checkChplVersion(lockFile);

    if !isDir(MASON_HOME) {
      mkdir(MASON_HOME, parents=true);
    }

    // generate list of dependencies and get src code
    var (sourceList, gitList) = genSourceList(lockFile);

    if lockFile.pathExists('external') {
      spackInstalled();
    }
    getSrcCode(sourceList, skipUpdate, show);

    getGitCode(gitList, show);

    // get compilation options including external dependencies
    var compopts = cmdLineCompopts;
    compopts.pushBack(getTomlCompopts(lockFile));
    // Compile Program
    if compileSrc(lockFile, binLoc, release, compopts,
                  projectHome, sourceList, gitList) {
      writeln("Build Successful\n");
    } else {
      throw new MasonError("Build Failed");
    }
  } else {
    writeln("Skipping Build... No changes to project");
  }
}


/* Compiles the program into the main project src
   folder. Requires that the main library file be
   named after the project folder in which it is
   contained */
proc compileSrc(lockFile: borrowed Toml, binLoc: string,
                release: bool, compopts: list(string),
                projectHome: string,
                sourceList: list(srcSource),
                gitList: list(gitSource)) : bool throws {

  const depPath = Path.joinPath(MASON_HOME, 'src');
  const gitDepPath = Path.joinPath(MASON_HOME, 'git');
  const project = lockFile["root.name"]!.s;
  const pathToProj = Path.replaceExt(Path.joinPath(projectHome,
                                                   'src',
                                                   project), 'chpl');

  const moveTo = Path.joinPath(projectHome, 'target', binLoc, project);

  if !isFile(pathToProj) {
    throw new MasonError("Mason could not find your project");
  } else {
    log.debugln("Starting to create compilation command");

    var cmd: list(string);
    cmd.pushBack("chpl");
    cmd.pushBack(pathToProj);
    cmd.pushBack("-o " + moveTo);

    cmd.pushBack(compopts);

    if release then cmd.pushBack("--fast");
    if sourceList.size > 0 {
      cmd.pushBack("--main-module");
      cmd.pushBack(project);
    }

    for flag in MasonPrereqs.chplFlags() {
      log.debugf("+compflag %s\n", flag);
      cmd.pushBack(flag);
    }

    log.debugf("Base command: %?\n", cmd);

    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, version) in srcSource.iterList(sourceList) {
      const nameVer = "%s-%s".format(name, version);
      // version of -1 specifies a git dep
      if version != "-1" {
        const depDir = Path.joinPath(depPath, nameVer);
        const depSrc = Path.replaceExt(Path.joinPath(depDir, "src", name),
                                       "chpl");

        log.debugf("Adding source dependency %s's flags\n", name);
        cmd.pushBack(depSrc);

        for flag in MasonPrereqs.chplFlags(depDir) {
          log.debugf("+compflag %s\n", flag);
          cmd.pushBack(flag);
        }
      }
    }

    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, branch, _y) in gitSource.iterList(gitList) {
      const gitDepSrc = Path.joinPath(gitDepPath, name + "-" + branch,
                                      'src', name + ".chpl");
      cmd.pushBack(gitDepSrc);
    }

    writef("Compiling [%s] target: %s\n",
            if release then "release" else "debug", project);

    // compile Program with deps
    const command = " ".join(cmd.these());
    log.debugln("Compilation command: " + command);
    var compilation = runWithStatus(command);
    if compilation != 0 {
      return false;
    }

    // Confirming File Structure
    return isFile(Path.joinPath(projectHome, 'target', binLoc, project));
  }
  return false;
}


/* Generates a list of tuples that holds the git repo
   url and the name for local mason dependency pool */
proc genSourceList(lockFile: borrowed Toml) {
  var sourceList: list(srcSource);
  var gitList: list(gitSource);
  log.infoln("Generating source list");
  for (name, package) in zip(lockFile.A.keys(), lockFile.A.values()) {
    log.debugln("name: "+name);
    if package!.tag == fieldtag.fieldToml {
      if name == "root" || name == "system" || name == "external" then continue;
      else {
        var toml = lockFile[name]!;
        var version = toml["version"]!.s;

        if toml.pathExists("rev") {
          var url = toml["source"]!.s;
          var revision = toml["rev"]!.s;

          var branch: string;
          // use branch if specified, else default to HEAD
          if toml.pathExists("branch") {
            branch = toml["branch"]!.s;
          } else {
            branch = "HEAD";
          }
          log.debugln("adding to gitList: "+name);
          gitList.pushBack(new gitSource(url, name, branch, revision));
        } else if toml.pathExists("source") {
          var source = toml["source"]!.s;
          log.debugln("adding to sourceList: "+name);
          sourceList.pushBack(new srcSource(source, name, version));
        }
      }
    }
  }
  return (sourceList, gitList);
}

/* Clones the git repository of each dependency into
   the src code dependency pool */
proc getSrcCode(sourceList: list(srcSource), skipUpdate, show) throws {
  var baseDir = MASON_HOME +'/src/';
  forall (srcURL, name, version) in srcSource.iterList(sourceList) {
    // version of -1 specifies a git dep
    if version != "-1" {
      const nameVers = name + "-" + version;
      const destination = baseDir + nameVers;
      if !depExists(nameVers) {
        if skipUpdate then
          throw new MasonError("Dependency cannot be installed when " +
                               "MASON_OFFLINE is set.");
        writeln("Downloading dependency: " + nameVers);
        var getDependency = "git clone -qn "+ srcURL + ' ' + destination +'/';
        var checkout = "git checkout -q v" + version;
        if show {
          getDependency = "git clone -n " + srcURL + ' ' + destination + '/';
          checkout = "git checkout v" + version;
        }
        runCommand(getDependency);
        gitC(destination, checkout);
      }

      // add prerequisites
      for prereq in MasonPrereqs.prereqs(destination) {
        MasonPrereqs.install(destination, prereq);
      }
    }
  }
}

proc getGitCode(gitList: list(gitSource), show) {
  if !isDir(MASON_HOME + '/git/') {
    mkdir(MASON_HOME + '/git/', parents=true);
  }
  var baseDir = MASON_HOME +'/git/';
  forall (srcURL, name, branch, revision) in gitSource.iterList(gitList) {
    const nameVers = name + "-" + branch;
    const destination = baseDir + nameVers;
    if !depExists(nameVers, '/git/') {
      writeln("Downloading dependency: " + nameVers);
      var getDependency = "git clone -qn "+ srcURL + ' ' + destination +'/';
      var checkout = "git checkout -q " + revision;
      if show {
        getDependency = "git clone -n " + srcURL + ' ' + destination + '/';
        checkout = "git checkout " + revision;
      }
      runCommand(getDependency);
      gitC(destination, checkout);
    } else {
      writeln("Checking out specified revision for " + nameVers + "...");

      var checkoutBranch = "git checkout -q " + revision;
      if show {
        checkoutBranch = "git checkout " + revision;
      }
      gitC(destination, checkoutBranch);
    }
  }
}

// Retrieves root table compopts, external compopts, and system compopts
proc getTomlCompopts(lock: borrowed Toml): list(string) throws {
  var compopts = new list(string);
  // Checks for compilation options are present in Mason.toml
  if const cmpFlags = lock.get["root.compopts"] {
    try {
      compopts.pushBack(parseCompilerOptions(cmpFlags));
    } catch {
      throw new MasonError("unable to parse compopts");
    }
  }

  // get the dependencies, if they exist
  for (name, package) in zip(lock.A.keys(), lock.A.values()) {
    log.debugln("name: "+name);
    if package!.tag != fieldtag.fieldToml then continue;
    if name == "root" || name == "system" || name == "external" then continue;
    if const depFlags = package!.get["compopts"] {
      try {
        compopts.pushBack(parseCompilerOptions(depFlags));
      } catch {
        throw new MasonError("unable to parse compopts for dependency " + name);
      }
    }
    if const system = package!.get["system"] {
        for (_, depInfo) in zip(system.A.keys(), system.A.values()) {
          for (k,v) in allFields(depInfo!) {
            var val = v!;
            select k {
              when "libs" do compopts.pushBack(parseCompilerOptions(val));
              when "include" do
                if val.s != "" then compopts.pushBack("-I" + val.s);
              otherwise continue;
            }
          }
        }
      }
  }

  if const exDeps = lock.get['external'] {
    for (_, depInfo) in zip(exDeps.A.keys(), exDeps.A.values()) {
      for (k,v) in allFields(depInfo!) {
        var val = v!;
        select k {
            when "libs" do compopts.pushBack("-L" + val.s);
            when "include" do compopts.pushBack("-I" + val.s);
            when "other" do compopts.pushBack("-I" + val.s);
            otherwise continue;
          }
      }
    }
  }
  if const pkgDeps = lock.get['system'] {
    for (_, depInfo) in zip(pkgDeps.A.keys(), pkgDeps.A.values()) {
      for (k,v) in allFields(depInfo!) {
        var val = v!;
        select k {
          when "libs" do compopts.pushBack(parseCompilerOptions(val));
          when "include" do if val.s != "" then compopts.pushBack("-I" + val.s);
          otherwise continue;
        }
      }
    }
  }
  return compopts;
}



// TODO: ideally fingerprinting uses a checksum, but that requires adding
// a new dependency to mason (openssl) which is overkill for now
// proc sha256(s): string {
//   use Crypto;
//   var hasher = new Hash(Digest.SHA256);
//   var buffer = new CryptoBuffer(s);
//   var digest = hasher.getDigest(buffer);
//   return digest.toHexString();
// }

proc printChplEnv(): string {
  const printchplenv = joinPath(MasonUtils.CHPL_HOME, "util", "printchplenv");
  var output = "";
  try {
    output = runCommand([printchplenv, "--all", "--internal", "--simple"],
                         quiet=true);
  } catch e {
    log.errorln("Could not run printchplenv to " +
                "get Chapel environment variables");
  }
  return output;
}
/*
  Returns the environment variables (and their values) which may affect
  the build process.

  This is not an exhaustive list of all environment variables which
  may affect the build process, but is intended to capture the most common
*/
proc getInterestingEnvVars(): string {
  const vars = ("CHPL_HOME",
                "CHPL_MODULE_PATH",
                "CHPL_CC_FLAGS", "CHPL_INCLUDE_PATH",
                "CHPL_LD_FLAGS", "CHPL_LIB_NAME", "CHPL_LIB_PATH");
  var output = "";
  for v in vars {
    const val = getEnv(v);
    output += v + "=" + val + "\n";
  }
  return output;
}

proc computeFingerprint(
  commandLineCompopts: list(string) = new list(string)
): string {
  var fingerprint = "";
  fingerprint += "MasonVersion=" + MASON_VERSION + "\n";
  fingerprint += "ChapelVersion=" + getChapelVersionStr() + "\n";
  fingerprint += printChplEnv();
  fingerprint += getInterestingEnvVars();
  fingerprint += "cmdline_compopts=" +
                 (" ".join(commandLineCompopts.these())) + "\n";
  // return sha256(fingerprint);
  return fingerprint;
}

/*
  Returns true if the fingerprint matches the stored fingerprint for the
  project, false otherwise. If no stored fingerprint exists, one is created.
*/
proc checkFingerprint(projectName:string,
                      fingerprintDir: string,
                      fingerprint: string): bool {
  const fingerprintFile = joinPath(fingerprintDir,
                                   "%s-%s".format(projectName, "fingerprint"));
  if !isFile(fingerprintFile) {
    if !isDir(fingerprintDir) then
      mkdir(fingerprintDir, parents=true);
    log.debugln("No previous fingerprint found, creating new fingerprint");
    const writer = openWriter(fingerprintFile);
    writer.write(fingerprint);
    return false;
  } else {
    const reader = openReader(fingerprintFile);
    const old = reader.readAll(string);
    reader.close();
    if old != fingerprint {
      log.debugln("Fingerprints do not match, rebuild required");
      // update fingerprint
      reader.close();
      const writer = openWriter(fingerprintFile);
      writer.write(fingerprint);
      return false;
    } else {
      log.debugln("Fingerprints match, no rebuild required");
      return true;
    }
  }
}

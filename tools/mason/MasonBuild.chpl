/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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


use ArgumentParser;
use FileSystem;
use List;
use Map;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonUpdate;
use MasonSystem;
use MasonExternal;
use MasonExample;
use Subprocess;
use TOML;

proc masonBuild(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonBuildHelpHandler());

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var forceFlag = parser.addFlag(name="force", defaultValue=false);
  var exampleOpts = parser.addOption(name="example", numArgs=0..);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);

  if passArgs.hasValue() && exampleOpts._present {
    throw new owned MasonError("Examples do not support `--` syntax");
  }

  var show = showFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var force = forceFlag.valueAsBool();
  var compopts: list(string);
  var example = false;
  var skipUpdate = MASON_OFFLINE;

  // when --example provided with or without a value
  if exampleOpts._present then example = true;

  if updateFlag.hasValue() {
    if updateFlag.valueAsBool() then skipUpdate = false;
    else skipUpdate = true;
  }

  if example {
    // compopts become example names. Build never runs examples
    for val in exampleOpts.values() do compopts.append(val);
    compopts.append("--no-run");
    if skipUpdate then compopts.append('--no-update');
                  else compopts.append('--update');
    if show then compopts.append("--show");
    if release then compopts.append("--release");
    if force then compopts.append("--force");
    // add expected arguments for masonExample
    compopts.insert(0,["example", "--example"]);
    masonExample(compopts.toArray());
  }
  else {
    if passArgs.hasValue() {
      for val in passArgs.values() do compopts.append(val);
    }
    const configNames = updateLock(skipUpdate);
    const tomlName = configNames[0];
    const lockName = configNames[1];
    buildProgram(release, show, force, compopts, tomlName, lockName);
  }
}

private proc checkChplVersion(lockFile : borrowed Toml) throws {
  const root = lockFile["root"]!;
  const (success, low, hi) = verifyChapelVersion(root);

  if success == false {
    throw new owned MasonError("Build failure: lock file expecting chplVersion " + prettyVersionRange(low, hi));
  }
}


proc buildProgram(release: bool, show: bool, force: bool, ref cmdLineCompopts: list(string),
                  tomlName="Mason.toml", lockName="Mason.lock") throws {

  try! {

    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd, tomlName);
    const toParse = open(projectHome + "/" + lockName, iomode.r);
    var lockFile = owned.create(parseToml(toParse));
    const projectName = lockFile["root"]!["name"]!.s;

    // --fast
    var binLoc = 'debug';
    if release then
      binLoc = 'release';


    // build on last modification
    if projectModified(projectHome, projectName, binLoc) || force {

      if isFile(projectHome + "/" + lockName) {

        // Make build files and check chapel version
        makeTargetFiles(binLoc, projectHome);
        checkChplVersion(lockFile);

        if isDir(MASON_HOME) == false {
          mkdir(MASON_HOME, parents=true);
        }

        // generate list of dependencies and get src code
        var sourceList = genSourceList(lockFile);

        if lockFile.pathExists('external') {
          spackInstalled();
        }
        //
        // TODO: Temporarily use `toArray` here because `list` does not yet
        // support parallel iteration, which the `getSrcCode` method _must_
        // have for good performance.
        //
        getSrcCode(sourceList, show);

        // get compilation options including external dependencies
        const compopts = getTomlCompopts(lockFile, cmdLineCompopts);
        // Compile Program
        if compileSrc(lockFile, binLoc, show, release, compopts, projectHome) {
          writeln("Build Successful\n");
        }
        else {
          throw new owned MasonError("Build Failed");
        }
        // Close memory
        toParse.close();
      }
      else {
        throw new owned MasonError("Cannot build: no Mason.lock found");
      }
    }
    else {
      writeln("Skipping Build... No changes to project");
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


/* Compiles the program into the main project src
   folder. Requires that the main library file be
   named after the project folder in which it is
   contained */
proc compileSrc(lockFile: borrowed Toml, binLoc: string, show: bool,
                release: bool, compopts: list(string), projectHome: string) : bool throws {

  const sourceList = genSourceList(lockFile);
  const depPath = MASON_HOME + '/src/';
  const project = lockFile["root"]!["name"]!.s;
  const pathToProj = projectHome + '/src/'+ project + '.chpl';
  const moveTo = ' -o ' + projectHome + '/target/'+ binLoc +'/'+ project;

  if !isFile(pathToProj) {
    throw new owned MasonError("Mason could not find your project");
  }
  else {
    var command: string = 'chpl ' + pathToProj + moveTo + ' ' + ' '.join(compopts.these());
    if release then command += " --fast";
    if sourceList.size > 0 then command += " --main-module " + project;

    for (_, name, version) in sourceList {
      var depSrc = ' ' + depPath + name + "-" + version + '/src/' + name + ".chpl";
      command += depSrc;
    }

    // Verbosity control
    if show then writeln("Compilation command: " + command);
    else {
      if release then writeln("Compiling [release] target: " + project);
      else writeln("Compiling [debug] target: " + project);
    }

    // compile Program with deps
    var compilation = runWithStatus(command);
    if compilation != 0 {
      return false;
    }

    // Confirming File Structure
    if isFile(projectHome + '/target/' + binLoc + '/' + project) then
      return true;
    else return false;
  }
  return false;
}


/* Generates a list of tuples that holds the git repo
   url and the name for local mason dependency pool */
proc genSourceList(lockFile: borrowed Toml) {
  var sourceList: list((string, string, string));
  for (name, package) in lockFile.A.items() {
    if package!.tag == fieldtag.fieldToml {
      if name == "root" || name == "system" || name == "external" then continue;
      else {
        var toml = lockFile[name]!;
        var version = toml["version"]!.s;
        var source = toml["source"]!.s;
        sourceList.append((source, name, version));
      }
    }
  }
  return sourceList;
}


/* Checks to see if dependency has already been
   downloaded previously */
proc depExists(dependency: string) {
  var repos = MASON_HOME +'/src/';
  var exists = false;
  for dir in listdir(repos) {
    if dir == dependency then
      exists = true;
  }
  return exists;
}

/* Clones the git repository of each dependency into
   the src code dependency pool */
proc getSrcCode(sourceListArg: list(3*string), show) {

  //
  // TODO: Temporarily use `toArray` here because `list` does not yet
  // support parallel iteration, which the `getSrcCode` method _must_
  // have for good performance.
  //
  var sourceList = sourceListArg.toArray();

  var baseDir = MASON_HOME +'/src/';
  forall (srcURL, name, version) in sourceList {
    const nameVers = name + "-" + version;
    const destination = baseDir + nameVers;
    if !depExists(nameVers) {
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
  }
}

// Retrieves root table compopts, external compopts, and system compopts
proc getTomlCompopts(lock: borrowed Toml, ref compopts: list(string)) {

  // Checks for compilation options are present in Mason.toml
  if lock.pathExists('root.compopts') {
    const cmpFlags = lock["root"]!["compopts"]!.s;
    compopts.append(cmpFlags);
  }

  if lock.pathExists('external') {
    const exDeps = lock['external']!;
    for (name, depInfo) in exDeps.A.items() {
      for (k,v) in allFields(depInfo!) {
        var val = v!;
        select k {
            when "libs" do compopts.append("-L" + val.s);
            when "include" do compopts.append("-I" + val.s);
            when "other" do compopts.append("-I" + val.s);
            otherwise continue;
          }
      }
    }
  }
  if lock.pathExists('system') {
    const pkgDeps = lock['system']!;
    for (name, dep) in pkgDeps.A.items() {
      var depInfo = dep!;
      compopts.append(depInfo["libs"]!.s);
      compopts.append("-I" + depInfo["include"]!.s);
    }
  }
  return compopts;
}

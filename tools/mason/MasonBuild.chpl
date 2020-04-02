/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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


private use List;
private use Map;

use TOML;
use Spawn;
use FileSystem;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonUpdate;
use MasonSystem;
use MasonExample;

proc masonBuild(args) throws {
  var show = false;
  var release = false;
  var force = false;
  var compopts: list(string);
  var opt = false;
  var example = false;
  var update = false;

  if args.size > 2 {

    //
    // This function is generic and may be instantiated with either a list
    // or an array as the type for "args". In the case of list, the
    // start index is 1, however in the case of an array, the `low` element
    // is 0. The below code is a stopgap.
    //
    var start = 3;
    var end = args.size; 
    if isArray(args) && args.eltType == string {
      start = args.domain.low + 2;
      end = args.domain.high;
    }

    for i in start..end {
      var arg = args[i];
      if opt == true {
        compopts.append(arg);
      }
      else if arg == '-h' || arg == '--help' {
        masonBuildHelp();
        exit(0);
      }
      else if arg == '--' {
        if example then
          throw new owned MasonError("Examples do not support `--` syntax");
        opt = true;
      }
      else if arg == '--release' {
        release = true;
      }
      else if arg == '--force' {
        force = true;
      }
      else if arg == '--show' {
        show = true;
      }
      else if arg == '--example' {
        example = true;
      }
      else if arg == '--update' {
        update = true;
      }
      // passed to UpdateLock
      else if arg == '--no-update' {
        continue;
      }
      else {
        compopts.append(arg);
      }
    }
  }
  if example {
    // compopts become test names. Build never runs examples
    compopts.append("--no-run");
    if update then compopts.append('--update');
    if hasOptions(args, '--no-update') then compopts.append('--no-update');
    if show then compopts.append("--show");
    if release then compopts.append("--release");
    if force then compopts.append("--force");
    masonExample(compopts);
  }
  else {
    var argsList = new list(string);
    for x in args do argsList.append(x);
    const configNames = UpdateLock(argsList);
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

    const cwd = getEnv("PWD");
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

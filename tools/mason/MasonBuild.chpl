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


use TOML;
use Spawn;
use FileSystem;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonUpdate;
use MasonSystem;

proc masonBuild(args) {
  var show = false;
  var release = false;
  var force = false;
  var compopts: [1..0] string;
  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonBuildHelp();
        exit();
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
      else {
        compopts.push_back(arg);
      }
    }
  }
  const configNames = UpdateLock(args);
  const tomlName = configNames[1];
  const lockName = configNames[2];
  buildProgram(release, show, force, compopts, tomlName, lockName);
}

private proc checkChplVersion(lockFile : Toml) {
  const root = lockFile["root"];
  const (success, low, hi) = verifyChapelVersion(root);

  if success == false {
    stderr.writeln("Build failure: lock file expecting chplVersion ", prettyVersionRange(low, hi));
    exit(1);
  }
}

proc buildProgram(release: bool, show: bool, force: bool, cmdLineCompopts: [?d] string,
                  tomlName="Mason.toml", lockName="Mason.lock") {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd, tomlName);
    const toParse = open(projectHome + "/" + lockName, iomode.r);
    var lockFile = new Owned(parseToml(toParse));
    const projectName = lockFile["root"]["name"].s;
    
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
        getSrcCode(sourceList, show);

        // get compilation options including external dependencies
        const compopts = getCompopts(lockFile, cmdLineCompopts);

        // Compile Program
        if compileSrc(lockFile, binLoc, show, release, compopts, projectHome) {
          writeln("Build Successful\n");
        }
        else {
          writeln("Build Failed");
          exit(1);
        }
        // Close memory
        toParse.close();
      }
      else {
        writeln("Cannot build: no Mason.lock found");
        exit(1);
      }
    }
    else {
      writeln("Skipping Build... No changes to project");
    }
  }
  catch e: MasonError {
    exit(1);  
  }
}


/* Compiles the program into the main project src
   folder. Requires that the main library file be
   named after the project folder in which it is
   contained */
proc compileSrc(lockFile: Toml, binLoc: string, show: bool,
                release: bool, compopts: [?dom] string, projectHome: string) : bool {

  const sourceList = genSourceList(lockFile);
  const depPath = MASON_HOME + '/src/';
  const project = lockFile["root"]["name"].s;
  const pathToProj = projectHome + '/src/'+ project + '.chpl';
  const moveTo = ' -o ' + projectHome + '/target/'+ binLoc +'/'+ project;

  if isFile(pathToProj) {
    var command: string = 'chpl ' + pathToProj + moveTo + ' ' + ' '.join(compopts);
    if release then command += " --fast";
    if sourceList.numElements > 0 then command += " --main-module " + project;

    for (_, name, version) in sourceList {
      var depSrc = ' ' + depPath + name + "-" + version + '/src/' + name + ".chpl";
      command += depSrc;
    }

    // Verbosity control
    if show then writeln(command);
    else writeln("Compiling "+ project);

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
  else {
    writeln("Mason could not find your project!");
    return false;
  }
}


/* Generates a list of tuples that holds the git repo
   url and the name for local mason dependency pool */
proc genSourceList(lockFile: Toml) {
  var sourceList: [1..0] (string, string, string);
  for (name, package) in zip(lockFile.D, lockFile.A) {
    if package.tag == fieldToml {
      if name == "root" || name == "system" then continue;
      else {
        var version = lockFile[name]["version"].s;
        var source = lockFile[name]["source"].s;
        sourceList.push_back((source, name, version));
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
proc getSrcCode(sourceList: [?d] 3*string, show) {
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

private proc getCompopts(lock: Toml, compopts: [?d] string) {

  // Checks for compilation options are present in Mason.toml
  if lock.pathExists('root.compopts') {
    const cmpFlags = lock["root"]["compopts"].s;
    compopts.push_back(cmpFlags);
  }
  // Get pkgconfig dependency compilation options
  if lock.pathExists('system') {
    const exDeps = lock['system'];
    for (name, depInfo) in zip(exDeps.D, exDeps.A) {
      compopts.push_back(depInfo["libs"].s);
      compopts.push_back("-I" + depInfo["include"].s);
    }
  }
  return compopts;
}

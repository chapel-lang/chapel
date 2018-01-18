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


proc masonBuild(args) {
  var show = false;
  var release = false;
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
      else if arg == '--show' {
        show = true;
      }
      else {
        compopts.push_back(arg);
      }
    }
  }
  UpdateLock(args);
  buildProgram(release, show, compopts);
}

private proc checkChplVersion(lockFile : Toml) {
  const root = lockFile["root"];
  const (success, low, hi) = verifyChapelVersion(root);

  if success == false {
    stderr.writeln("Build failure: lock file expecting chplVersion ", prettyVersionRange(low, hi));
    exit(1);
  }
}

proc buildProgram(release: bool, show: bool, compopts: [?d] string) {
  if isFile("Mason.lock") {

    // --fast
    var binLoc = 'debug';
    if release then
      binLoc = 'release';

    // Make Binary Directory
    makeTargetFiles(binLoc);

    // Install dependencies into $MASON_HOME/src
    var toParse = open("Mason.lock", iomode.r);
    var lockFile = parseToml(toParse);
    checkChplVersion(lockFile);

    if isDir(MASON_HOME) == false {
      mkdir(MASON_HOME, parents=true);
    }

    var sourceList = genSourceList(lockFile);
    getSrcCode(sourceList, show);

    // Checks if dependencies exist and retrieves them for compilation
    if lockFile.pathExists('root.compopts') {
      const cmpFlags = lockFile["root"]["compopts"].s;
      compopts.push_back(cmpFlags);
    }

    // Compile Program
    if compileSrc(lockFile, binLoc, show, release, compopts) {
      writeln("Build Successful\n");
    }
    else {
      writeln("Build Failed");
      exit(1);
    }
    // Close memory
     delete lockFile;
     toParse.close();
  }
  else writeln("Cannot build: no Mason.lock found");
}


/* Creates the rest of the project structure */
proc makeTargetFiles(binLoc: string) {
  if !isDir('target') {
    mkdir('target');
  }
  if !isDir('target/' + binLoc) {
    mkdir('target/' + binLoc);

    // TODO:
    //mkdir('target/' + binLoc + '/tests');
    //mkdir('target/'+ binLoc + '/examples');
    //mkdir('target/' + binLoc + '/benches');
  }
}


/* Compiles the program into the main project src
   folder. Requires that the main library file be
   named after the project folder in which it is
   contained */
proc compileSrc(lockFile: Toml, binLoc: string, show: bool,
                release: bool, compopts: [?dom] string) : bool {
  var sourceList = genSourceList(lockFile);
  var depPath = MASON_HOME + '/src/';
  var project = lockFile["root"]["name"].s;
  var pathToProj = 'src/'+ project + '.chpl';
  var moveTo = ' -o target/'+ binLoc +'/'+ project;

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
    if isFile('target/' + binLoc + '/' + project) then
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
      if name == "root" then continue;
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

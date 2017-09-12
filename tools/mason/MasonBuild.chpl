/*
 * Copyright 2004-2017 Cray Inc.
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
  UpdateLock();
  BuildProgram(release, show, compopts);
}


proc BuildProgram(release: bool, show: bool, compopts: [?d] string) {
  if isFile("Mason.lock") {

    // --fast
    var binLoc = 'debug';
    if release then
      binLoc = 'release';

    // Make Binary Directory
    makeTargetFiles(binLoc);

    //Install dependencies into .mason/src
    var toParse = open("Mason.lock", iomode.r);
    var lockFile = parseToml(toParse);
    var sourceList = genSourceList(lockFile);
    getSrcCode(sourceList, show);

    // Compile Program
    if compileSrc(lockFile, binLoc, show, release, compopts) {
      writeln("Build Successful\n");
    }
    else writeln("Build Failed");

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
  var depPath = MASON_HOME + '/.mason/src/';
  var project = lockFile["root"]["name"].s;
  var pathToProj = 'src/'+ project + '.chpl';
  var moveTo = ' -o target/'+ binLoc +'/'+ project;
 
  if isFile(pathToProj) {
    var command: string = 'chpl ' + pathToProj + moveTo + ' ' + ' '.join(compopts);
    if release then command += " --fast";

    for dep in sourceList {
      var depSrc = ' -M '+ depPath + dep(2) + '/src';
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
  var sourceList: [1..0] (string, string);
  for (name, package) in zip(lockFile.D, lockFile.A) {
    if package.tag == fieldToml {
      if name == "root" then continue;
      else {
        var version = lockFile[name]["version"].s;
        var source = lockFile[name]["source"].s;
        sourceList.push_back((source, name+'-'+version));
      }
    }
  }
  return sourceList;
}

/* Clones the git repository of each dependency into
   the src code dependency pool */
proc getSrcCode(sourceList: [?d] 2*string, show) {
  var destination = MASON_HOME +'/.mason/src/';
  forall source in sourceList {
    if !depExists(source(2)) {
      var version = source(2).split('-');
      writeln("Downloading dependency: " + source(2));
      var getDependency = "git clone -qn "+source(1)+' '+destination+source(2)+'/';
      var checkout = "git -C "+ destination+source(2) + " checkout -q v"+version(2);
      if show {
	getDependency = "git clone -n "+source(1)+' '+destination+source(2)+'/';
	checkout = "git -C "+ destination+source(2) + " checkout v"+version(2);
      }
      runCommand(getDependency);
      runCommand(checkout);
    }
  }
}

/*
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


use MasonUtils;
use Spawn;
use FileSystem;
use TOML;
use MasonEnv;
use MasonNew;
use MasonModify;
use Random;

/* Top Level procedure that gets called from mason.chpl that takes in arguments from command line.
   Returns the help output in '-h' or '--help' exits in the arguments.
   If --dry-run is passed then it checks to see if the package is able to be published.
   Takes in the username of the package owner as an argument
 */
proc masonPublish(args: [] string) throws {
  try! {
    if hasOptions(args, "-h", "--help") {
      masonPublishHelp();
      exit(0);
    }
    var dry = false;
    var registryPath = '';
    var username = getUsername();
    var isLocal = false;

    const badSyntaxMessage = 'Arguments do not meet the "mason publish [options] <registry>" syntax';
    if args.size > 4 {
      throw new owned MasonError(badSyntaxMessage);
    }
    for arg in args[2..] {
      if arg == '--dry-run' then dry = true;
      else registryPath = arg;
    }
    if registryPath.isEmpty() {
      registryPath = MASON_HOME;
    }
    else {
      isLocal = isRegistryPathLocal(registryPath);
    }

    if checkRegistryPath(registryPath, isLocal) {
      if dry {
        dryRun(username, registryPath, isLocal);
      }
      else {
        publishPackage(username, registryPath, isLocal);
      }
    }
    else {
      writeln(badSyntaxMessage);
      writeln('See "mason publish -h" for more details');
      exit(0);
     }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* Uses the existence of a colon to see if a passed registryPath is a local or remote registryPath
 */
proc isRegistryPathLocal(registryPath : string) throws {
  return registryPath.find(":") == 0; 
}

/* When passed a registryPath and whether or not that registryPath is a local or remote registryPath,
   the function checks to make sure that it is a valid registryPath to a mason-registry
   by checking the existence of the Bricks
 */
proc checkRegistryPath(registryPath : string, trueIfLocal : bool) throws {
  try! {
    if registryPath == MASON_HOME then return true;
    if !exists('.git') {
      throw new owned MasonError(registryPath + ' is not a local git repository.');
    }
    if trueIfLocal {
      if exists(registryPath) && exists(registryPath + "/Bricks/") {
        return true;
      }
      else {
        throw new owned MasonError(registryPath + " is not a valid path");
        exit(0);
      }
    }
    else {
      var command = ('git ls-remote ' + registryPath).split();
      var checkRemote = spawn(command, stdout=PIPE);
      checkRemote.wait();
      if checkRemote.exit_status == 0 then return true;
      else {
        throw new owned MasonError(registryPath + " is not a valid remote path");
        exit(0);
      }
    }
  }
  catch e {
    writeln(e.message());
    exit(0);
  }
}
/* Main Script that goes through the act of publishing the package to the mason registry.
   Takes the package owners GitHub username as input will throw errors through command
   line git commands if any of the git calls fails.
 */
proc publishPackage(username: string, registryPath : string, isLocal : bool) throws {
  try! {
    const packageLocation = absPath(here.cwd());
    var stream = makeRandomStream(int);
    var uniqueDir = stream.getNext(): string;
    const name = getPackageName();
    var safeDir = '';

    if isLocal then safeDir = registryPath;
    else {
      safeDir = MASON_HOME + '/tmp/' + name + '-' + uniqueDir;
    }
  
    if !isLocal {
      if !exists(MASON_HOME + '/tmp') then mkdir(MASON_HOME + '/tmp');
      mkdir(safeDir);
    }
 
    if !isLocal {
      cloneMasonReg(username, safeDir, registryPath);
      branchMasonReg(username, name, safeDir, registryPath);
    }
  
    addPackageToBricks(packageLocation, safeDir, name, registryPath, isLocal);

    if !isLocal {
      gitC(safeDir + "/mason-registry", "git add .");
      gitC(safeDir + "/mason-registry", "git commit -m '" + name + "'");
      gitC(safeDir + "/mason-registry", 'git push --set-upstream origin ' + name, true);
      rmTree(safeDir + '/');
      writeln('--------------------------------------------------------------------');
      writeln('Go to the above link to open up a Pull Request to the mason-registry');
     }
    else {
      gitC(safeDir, 'git add Bricks/' + name);
      gitC(safeDir, "git commit -m '" + name + "'" );
    }
    
  }
  catch e {
    writeln(e.message());
  }
}

/* If --dry-run is passed then it takes the username and checks to see if the mason-registry is forked
 and the package has a git remote origin. If both exist then the package can be published.
 */
proc dryRun(username: string, registryPath : string, isLocal : bool) throws {
  if !isLocal {
    var fork = true;
    var remoteCheck = checkIfForkExists(username: string);
    if remoteCheck == 1 {
      fork = false;
    }
    var git = false;
    if doesGitOriginExist() {
      git = true;
    }
    if git && fork {
      writeln('Package can be published to the mason-registry');
      writeln('Commands that will be run:');
      writeln('> git clone git:github.com:[username]/mason-registry mason-registry');
      writeln('> git checkout -b [package name]');
      writeln('Package Name will be added to the Bricks in the mason-registry');
      writeln('> git add .');
      writeln('> git commit -m [package name]');
      writeln('> git push --set-upstream origin [package name]');
      exit(0);
    }
    else {
      if fork == false {
        throw new owned MasonError('mason-registry is not forked on your GitHub');
      }
      else {
        throw new owned MasonError('Package does not gave a git origin');
      }
    }
  }
  else {
    if checkRegistryPath(registryPath, isLocal) {
      writeln('Package can be published to local registry');
      exit(0);
    }
    else {
      throw new owned MasonError(registryPath + ' is not a valid registryPath to a local registry.');
    }
  }
}

/* Opens Spawn call to get username for the mason registry fork
 */
private proc usernameCheck(username: string) {
  const gitRemote = 'git ls-remote https://github.com/';
  var usernameCheck = runWithStatus(gitRemote + username + '/mason-registry');
  return usernameCheck;
}

/* Runs Commands to see if Fork of mason-registry exists under the username
 */
private proc checkIfForkExists(username: string) {
  var getFork = ('git ls-remote https://github.com/' + username + '/mason-registry');
  var p = runWithProcess(getFork, false);
  return p.exit_status;
}

/* Gets the GitHub username of the user, by parsing from the remote origin url.
 */
private proc getUsername() {
  var usernameUrl = gitUrl();
  var tail = usernameUrl.find("/")-1: int;
  var head = usernameUrl.find(":")+1: int;
  var username = usernameUrl(head..tail);
  return username;
}

/* Clones the mason registry fork from the users repo. Takes username as input.
 */
proc cloneMasonReg(username: string, safeDir : string, registryPath : string) throws {
  try! {
    if registryPath == MASON_HOME {
      const gitClone = 'git clone --quiet git@github.com:';
      var ret = gitC(safeDir, gitClone  + username + "/mason-registry mason-registry", false);
      return ret;
    }
    else {
      const gitRegistryPath = 'git clone --quiet ';
      var gitCall = gitC(safeDir, gitRegistryPath + registryPath + ' mason-registry', false);
      return gitCall;
    }
  }
  catch {
    writeln('Error cloning the fork of mason-registry');
    writeln('Make sure you have forked the mason-registry on GitHub');
    exit(1);
  }
}

/* Checks to see if 'git config --get remote.origin.url' exists
 */
proc doesGitOriginExist() {
  var urlExists = runCommand("git config --get remote.origin.url", true);
  return !urlExists.isEmpty();
}

/* Procedure that returns the url of the git remote origin
 */
private proc gitUrl() {
  var url = runCommand("git config --get remote.origin.url", true);
  return url;
}

/* Takes the git username and creates a new branch of the mason registry users fork,
  name or branch is taken from the Mason.toml of the mason package.
 */
proc branchMasonReg(username: string, name: string, safeDir: string, registryPath : string) throws {
  try! {
    const branchCommand = "git checkout --quiet -b  "+ name: string;
    var ret = gitC(safeDir + '/mason-registry', branchCommand, false);
    return ret;
  }
  catch {
    writeln('Error branching the registry, make sure you have a remote origin set up');
    exit(1);
  }
}

/* Gets name from the Mason.toml
 */
proc getPackageName() throws {
  try! {
    const toParse = open("Mason.toml", iomode.r);
    var tomlFile = new owned(parseToml(toParse));
    const name = tomlFile['brick']['name'].s;
    return name;
  }
  catch {
    writeln('Error getting the name of your package, ensure your package is a mason project');
    exit(1);
  }
}

/* Adds package to the Bricks of the mason-registry branch and then adds the version.toml
 with the source url of the package's GitHub repo.
 */
private proc addPackageToBricks(projectLocal: string, safeDir: string, name : string,registryPath : string, isLocal : bool) throws {
  try! {
    const toParse = open(projectLocal+ "/Mason.toml", iomode.r);
    var tomlFile = new owned(parseToml(toParse));
    const versionNum = tomlFile['brick']['version'].s;

    if !isLocal {
      mkdir(safeDir + "/mason-registry/Bricks/" + name);
      const baseToml = tomlFile;
      var newToml = open(safeDir + "/mason-registry/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
      var tomlWriter = newToml.writer();
      const url = gitUrl();
      baseToml["brick"].set("source", url[1..url.length-1]);
      tomlWriter.write(baseToml);
      tomlWriter.close();
    }
    else {
      mkdir(safeDir + "/Bricks/" + name);
      const baseToml = tomlFile;
      var newToml = open(safeDir + "/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
      var tomlWriter = newToml.writer();
      baseToml["brick"].set("source", projectLocal);
      tomlWriter.write(baseToml);
      tomlWriter.close();
      const gitMessageString = ('git tag -a v' + versionNum + ' -m  "' + name + '"');
      gitC(projectLocal, gitMessageString);
    }
  }
  catch {
    writeln('ERROR: ' + name + ' already exists in the Bricks');
    if !isLocal then rmTree(safeDir + '/');
    exit(1);
  }
}

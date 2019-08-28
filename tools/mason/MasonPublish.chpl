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
use MasonUpdate;
private use List;
use MasonBuild;

/* Top Level procedure that gets called from mason.chpl that takes in arguments from command line.
   Returns the help output in '-h' or '--help' exits in the arguments.
   If --dry-run is passed  then it checks to see if the package is able to be published.
   Takes in the username of the package owner as an argument
 */
proc masonPublish(args: [?d] string) {
  var listArgs: list(string);
    for x in args do listArgs.append(x);
    masonPublish(listArgs);
}

proc masonPublish(ref args: list(string)) throws {
  try! {
    if hasOptions(args, "-h", "--help") {
      masonPublishHelp();
      exit(0);
    }
    var dry = hasOptions(args, "--dry-run");
    var checkFlag = hasOptions(args, '--check');
    var registryPath = '';
    var username = getUsername();
    var isLocal = false;

    const badSyntaxMessage = 'Arguments does not follow "mason publish [options] <registry>" syntax';
    if args.size > 5 {
      throw new owned MasonError(badSyntaxMessage);
    }

    if args.size > 2 {
      var potentialPath = args.pop();
      if (potentialPath != '--dry-run') && (potentialPath != '--no-update') && (potentialPath != '--check') {
        registryPath = potentialPath;
      }
      args.append(potentialPath);
    }

    if registryPath.isEmpty() {
      registryPath = MASON_HOME;
    }
    else {
      isLocal = isRegistryPathLocal(registryPath);
    }

    if checkFlag {
      check(username, registryPath, isLocal);
    }

    updateRegistry('Mason.toml', args);
    if !isLocal && !doesGitOriginExist() && !dry {
      throw new owned MasonError('Your package must have a git origin remote in order to publish to a remote registry.');
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
        throw new owned MasonError(registryPath + " is not a valid path to a local mason-registry.");
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

    const version = addPackageToBricks(packageLocation, safeDir, name, registryPath, isLocal);
    var command : list(string);
    var  message = ' "Adding %s package to registry via mason publish"'.format(version);
    var gitString = ("git commit -m").split();
    command.append('git');
    command.append('commit');
    command.append('-m');
    command.append(message);

    if !isLocal {
      gitC(safeDir + "/mason-registry", "git add .");
      commitSubProcess(safeDir + '/mason-registry', command);
      gitC(safeDir + "/mason-registry", 'git push --set-upstream origin ' + name, true);
      rmTree(safeDir + '/');
      writeln('--------------------------------------------------------------------');
      writeln('Go to the above link to open up a Pull Request to the mason-registry');
     }
    else {
      gitC(safeDir, 'git add Bricks/' + name);
      commitSubProcess(safeDir, command);
      writeln('Successfully published package to ' + registryPath);
    }

  }
  catch e {
    writeln(e.message());
  }
}

/* Subprocess function designed to pass a message to 'git commit' without get the string
 split by the MasonUtils runCommand()/
 */
private proc commitSubProcess(dir, command) throws {
  var spawnArgs = [command[1], command[2], command[3], command[4]];
  const oldDir = here.cwd();
  here.chdir(dir);
  var commitSpawn = spawn(spawnArgs, stdout=PIPE, stderr=PIPE);
  commitSpawn.wait();
  here.chdir(oldDir);
  return commitSpawn;
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
      writeln('> git commit -m [package info]');
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
  var usernameCheck = runWithStatus(gitRemote + username + '/mason-registry', false);
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
    throw new owned MasonError('Error cloning the fork of mason-registry. Make sure you have forked the mason-registry on GitHub');
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
    throw new owned MasonError('Error branching the registry, make sure you have a remote origin set up');
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
    throw new owned MasonError('Issue getting the name of your package, ensure your package is a mason project.');
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
      if !exists(safeDir + '/mason-registry/Bricks/') {
        throw new owned MasonError('Registry does not have the expected structure. Ensure your registry has a Bricks directory.');
      }
      if !exists(safeDir + "/mason-registry/Bricks/" + name) {
        mkdir(safeDir + "/mason-registry/Bricks/" + name);
      }
      if !exists(safeDir + '/mason-registry/Bricks/' + name + "/" + versionNum + ".toml") {
        const baseToml = tomlFile;
        var newToml = open(safeDir + "/mason-registry/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
        var tomlWriter = newToml.writer();
        const url = gitUrl();
        baseToml["brick"].set("source", url[1..url.length-1]);
        tomlWriter.write(baseToml);
        tomlWriter.close();
        return name + '@' + versionNum;
      }
      else {
        throw new owned MasonError('A package with that name and version number already exists in the Bricks.');
        exit(1);
      }
    }
    else {
      if !exists(safeDir + '/.git') {
        throw new owned MasonError('Unable to publish your package to the registry, make sure your package is a git repository.');
        exit(1);
      }
      if !exists(safeDir + '/Bricks/' + name) {
        mkdir(safeDir + "/Bricks/" + name);
      }
      if !exists(safeDir + "/Bricks/" + name + "/" + versionNum + ".toml") {
        const baseToml = tomlFile;
        var newToml = open(safeDir + "/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
        var tomlWriter = newToml.writer();
        baseToml["brick"].set("source", projectLocal);
        tomlWriter.write(baseToml);
        tomlWriter.close();
        const gitMessageString = ('git tag -a v' + versionNum + ' -m  "' + name + '"');
        gitC(projectLocal, gitMessageString);
        return name + '@' + versionNum;
      }
      else {
        throw new owned MasonError('A package with that name and version already exists in the Bricks.');
        exit(1);
      }
    }
  }
  catch e {
    writeln(e.message());
    exit(1);
  }
}
/* check is a function to run a quick list of checks of the package, the registry path, and other issues that may
   prevent a package from being published to a registry.
 */
proc check(username : string, path : string, trueIfLocal) throws {
  const spacer = '------------------------------------------------------';
  const package = (ensureMasonProject(here.cwd(), 'Mason.toml') == 'true');
  const projectCheckHome = here.cwd();
  writeln('Mason Project Check:');
  if !package {
    writeln('    Could not find your configuration file (Mason.toml) (FAILED)');
    writeln('    Ensure your project is a mason package');
    }
  else {
    writeln('    Package is a Mason package and has a Mason.toml (PASSED)');
  }
  writeln(spacer);

  if package {
    writeln('Main Module Check:');
    if moduleCheck(projectCheckHome) {
      writeln('    Your package has only one main module, can be published to a registry. (PASSED)');
    }
    else writeln('    Packages with more than one modules cannot be published. (FAILED)');
    writeln(spacer);
  }

  if package {
    writeln('Git Remote Check:');
    if doesGitOriginExist() {
      writeln('    Package has a git remote origin and can be published to a remote registry (PASSED)');
      writeln('    Remote Origin: ' + getRemoteOrigin());
    }
    else {
      writeln('    Package has no remote origin and cannot be publish to a registry with path:' + path + ' (FAILED)');
    }
    writeln(spacer);
  }
  writeln('Checking Registry with ' + path + ' path.');
  registryPathCheck(path, username, trueIfLocal);
  writeln(spacer);
  writeln('The current mason environment is:');
  returnMasonEnv();
  if MASON_REGISTRY.size == 1 {
    writeln('    In order to use a local registry, ensure that MASON_REGISTRY points to the path.');
  }
  writeln(spacer);
  if package {
    writeln('Attempting to build package using following options:');
    writeln('    show = false');
    writeln('    release = false');
    writeln('    force = true');
    writeln('    opt = false');
    writeln('    example = false');
    writeln('If these are different than what is required to build your package you can disregard this check');
    attemptToBuild();
  }
  writeln(spacer);
  writeln();
  writeln();
 
  exit(0);
}

/* Attempts to build the package/
 */
private proc attemptToBuild() throws {
  const buildArgs = ['mason', 'build', '--force'];
  masonBuild(buildArgs);
}

/* registryPathCheck accepts the path, username, and trueIfLocal in order to check whether the registry that someone
   is trying to publish to is properly set up and has the correct structure.
 */
private proc registryPathCheck(path : string, username : string, trueIfLocal : bool) throws {
  if path == MASON_HOME {
    var forkCheck = usernameCheck(username);
    if forkCheck == 0 {
      writeln('    The mason-registry is forked under username: ' + username + ' (PASSED)');
    }
    else {
      writeln('    You must have a fork of the mason-registry to publish a package (FAILED)');
    }
  }
  else {
    if trueIfLocal {
      const isLocalGit = exists(path + '/.git');
      const hasBricks = exists(path + '/Bricks/');
      if !isLocalGit {
        writeln('   Registry with path ' + path + ' is not a git repository. (FAILED)');
        writeln("   Local registrys must be git repositorys in order to publish");
      }
      else if !hasBricks {
        writeln('   The registry with path ' + path + ' does not have proper registry structure (FAILED)');
        writeln("   A registry must have a /Bricks/ directory to be a valid registry");
      }
      else {
        writeln('   The local registry with path ' + path + ' is a valid registry to be publish too (PASSED)');
      }
    }
  }
}

/* Grabs the remote origin of the package
 */
private proc getRemoteOrigin() {
  const packageDir = here.cwd();
  var gitRemoteOrigin = gitC(packageDir, 'git config --get remote.origin.url', true);
  return gitRemoteOrigin;
}

/* Makes sure that the directory `mason publish --check is called in is a mason package
 */
private proc ensureMasonProject(cwd : string, tomlName="Mason.toml") : string {
  const (dirname, basename) = splitPath(cwd);
  if dirname == '/' {
    return 'false';
  }
  const tomlFile = joinPath(cwd, tomlName);
  if exists(tomlFile) {
    return 'true';
  }
  return ensureMasonProject(dirname, tomlName);
}

/* Checks to make sure the package only has one main module
 */
private proc moduleCheck(projectHome : string) throws {
  const subModules = listdir(projectHome + '/src');
  if subModules.size > 1 then return false;
  else return true;
}

/* Returns the mason env
 */
private proc returnMasonEnv() {
  const fakeArgs = ['mason', 'env'];
  masonEnv(fakeArgs);
}
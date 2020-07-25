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
    var ci = hasOptions(args, '--ci-check');
    var update = hasOptions(args, '--update');
    var noUpdate = hasOptions(args, '--no-update');
    var skipUpdate = MASON_OFFLINE;
    if update {
      skipUpdate = false;
    }
    if noUpdate {
      skipUpdate = true;
    }

    const badSyntaxMessage = 'Arguments does not follow "mason publish [options] <registry>" syntax';
    if args.size > 5 {
      throw new owned MasonError(badSyntaxMessage);
    }

    if args.size > 2 {
      var potentialPath = args.pop();
      if (potentialPath != '--dry-run') && (potentialPath != '--no-update') && (potentialPath != '--check') && (potentialPath != '--update') && (potentialPath != '--ci-check') {
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

    if checkFlag || ci {
      if ci then check(username, registryPath, isLocal, ci);
      else {
        check(username, registryPath, isLocal, ci);
      }
    }
    if ((MASON_OFFLINE  && !update) || noUpdate == true) && !falseIfRemotePath() {
      if !isLocal {
        throw new owned MasonError('You cannot publish to a remote repository when MASON_OFFLINE is set to true or "--no-update" is passed, override with --update');
      }
      else updateRegistry(skipUpdate);
    }

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
  catch e : MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* Uses the existence of a colon to see if a passed registryPath is a local or remote registryPath
 */
proc isRegistryPathLocal(registryPath : string) throws {
  return registryPath.find(":") == -1;
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
    var stream = createRandomStream(int);
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
    var tomlFile = owned.create(parseToml(toParse));
    const name = tomlFile['brick']!['name']!.s;
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
    var tomlFile:owned class? = owned.create(parseToml(toParse));
    const versionNum = tomlFile!['brick']!['version']!.s;
    if !isLocal {
      if !exists(safeDir + '/mason-registry/Bricks/') {
        throw new owned MasonError('Registry does not have the expected structure. Ensure your registry has a Bricks directory.');
      }
      if !exists(safeDir + "/mason-registry/Bricks/" + name) {
        mkdir(safeDir + "/mason-registry/Bricks/" + name);
      }
      if !exists(safeDir + '/mason-registry/Bricks/' + name + "/" + versionNum + ".toml") {
        const baseToml = tomlFile:owned class;
        var newToml = open(safeDir + "/mason-registry/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
        var tomlWriter = newToml.writer();
        const url = gitUrl();
        baseToml["brick"]!.set("source", url[0..<url.size-1]);
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
        const baseToml = tomlFile:owned class;
        var newToml = open(safeDir + "/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
        var tomlWriter = newToml.writer();
        baseToml["brick"]!.set("source", projectLocal);
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
proc check(username : string, path : string, trueIfLocal : bool, ci : bool) throws {
  const spacer = '------------------------------------------------------';
  const package = (ensureMasonProject(here.cwd(), 'Mason.toml') == 'true');
  const projectCheckHome = here.cwd();
  var packageTest = true;
  var moduleTest = true;
  var remoteTest = true;
  var exampleTest = true;
  var testTest = true;

  var gitTagTest = true;
  var masonFieldsTest = true;
  var licenseTest = true;

  writeln('Mason Project Check:');
  if !package {
    writeln('   Could not find your configuration file (Mason.toml) (FAILED)');
    writeln('   Ensure your project is a mason package');
    packageTest = false;
    }
  else {
    writeln('   Package is a Mason package and has a Mason.toml (PASSED)');
  }
  writeln(spacer);

  if package {
    writeln('Main Module Check:');
    if moduleCheck(projectCheckHome) {
      writeln('   Your package has only one main module, can be published to a registry. (PASSED)');
    }
    else {
      writeln('   Packages with more than one modules cannot be published. (FAILED)');
      moduleTest = false; 
    }
    writeln(spacer);
  }

  if package {
    writeln('Checking for fields in manifest file:');
    const manifestResults = masonTomlFileCheck(projectCheckHome);
    if manifestResults[0] {
      writeln('   All fields present in manifest file, can be published to a registry. (PASSED)');
    } else {
      writeln('   Missing fields in manifest file (Mason.toml). (FAILED)');
      writeln('   The missing fields are as follows: ');
      const missingFields = manifestResults[1];
      for field in missingFields do writeln('   %s'.format(field));
      masonFieldsTest = false;
    }
    writeln(spacer);
  }

  if package {
    writeln('Checking for examples:');
    if exampleCheck(projectCheckHome) {
      writeln('   Found examples in the package, can be published to a registry. (PASSED)');
    } else {
      writeln('   No examples found in package. (WARNING)');
      exampleTest = false;
    }
    writeln(spacer);
  }
  
  if package {
    writeln('Checking for tests:');
    if testCheck(projectCheckHome) {
      writeln('   Found tests in the package, can be published to a registry. (PASSED)');
    } else {
      writeln('   No tests found in package. (FAILED)');
      testTest = false;
    }
    writeln(spacer);
  }

  if package {
    writeln('Checking git tag version formatting:');
    const tagResults = gitTagVersionCheck(projectCheckHome);
    if tagResults[0] {
      writeln('   Valid git tag version formatting, can be published to a registry. (PASSED)');
    } else {
      writeln('   Invalid git tag version formatting. (FAILED)');
      const listTags = tagResults[1];
      const foundVersion = tagResults[2];
      writeln('   Expected tag version: %s'.format(foundVersion));
      writeln('   Tags found: ');
      for tag in listTags do writeln('   %s'.format(tag));
      gitTagTest = false;
    }
    writeln(spacer);
  }
  
  if package {
    writeln('Checking for license:');
    var validLicenseCheck = checkLicense(projectCheckHome);
    if validLicenseCheck[0] {
      writeln('   Found valid license in manifest file. (PASSED)');
    } else {
      writeln('   Invalid license name: "' + validLicenseCheck[1] + '". Please use a valid name from ' +
          'SPDX license list. (FAILED)');
      licenseTest = false;
    }
    writeln(spacer);
  }

  if package && !ci {
    writeln('Git Remote Check:');
    if doesGitOriginExist() {
      writeln('   Package has a git remote origin and can be published to a remote registry (PASSED)');
      writeln('   Remote Origin: ' + getRemoteOrigin());
    }
    else {
      writeln('   Package has no remote origin and cannot be publish to a registry with path:' + path + ' (FAILED)');
      remoteTest = false;
    }
    writeln(spacer);
  }
  writeln('Checking Registry with ' + path + ' path.');
  var registryTest = registryPathCheck(path, username, trueIfLocal);
  writeln(spacer);
  writeln('The current mason environment is:');
  returnMasonEnv();
  if MASON_REGISTRY.size == 1 {
    writeln('   In order to use a local registry, ensure that MASON_REGISTRY points to the path.');
  }
  
  writeln(spacer);
  if package && !ci {
    writeln('Attempting to build package using following options:');
    writeln('   show = false');
    writeln('   release = false');
    writeln('   force = true');
    writeln('   opt = false');
    writeln('   example = false');
    writeln('If these are different than what is required to build your package you can disregard this check');
    attemptToBuild();
  }
  writeln(spacer);
  writeln();
  writeln();
  writeln();
  writeln();
  writeln();
  writeln('RESULTS');
  writeln(spacer);

  if packageTest && remoteTest && moduleTest && registryTest && testTest 
    && licenseTest && gitTagTest && masonFieldsTest {
    writeln('(PASSED) Your package is ready to publish');
  }
  else {
    if !packageTest {
      writeln('(FAILED) Your package does not have to proper package structure');
    }
    if !moduleTest {
      writeln('(FAILED) Your package has more than one main module');
    }
    if !masonFieldsTest {
      writeln('(FAILED) Your package has missing fields in manifest file (Mason.toml)');
    }
    if !licenseTest {
      writeln('(FAILED) Your package does not have valid license name.');
    }
    if !exampleTest {
      writeln('(WARNING) Your package does not have examples');
    }
    if !testTest {
      writeln('(FAILED) Your package does not have tests');
    }
    if !registryTest {
      writeln('(FAILED) Your proposed registry is not a valid registry or path to a registry');
    }
    if !remoteTest {
      writeln('(FAILED) Your package has no remote origin and cannot be published');
    }
    if !gitTagTest {
      writeln('(FAILED) Your package has invalid git tag version formatting');
    }
  }

  writeln(spacer);

  if ci {
    if package && moduleCheck(projectCheckHome) && testCheck(projectCheckHome) 
    && checkLicense(projectCheckHome)[0] && masonTomlFileCheck(projectCheckHome)[0]
    && gitTagVersionCheck(projectCheckHome)[0] {
      attemptToBuild();
      exit(0);
    }
    else {
      writeln('New package does not have the proper structure.');
      exit(1);
    }
  }
  exit(0);
}

/* Validate license with that of SPDX list */
private proc checkLicense(projectHome: string) throws {
  var foundValidLicense = false;
  var defaultLicense = "None";
  if exists(joinPath(projectHome, "Mason.toml")) {
    const toParse = open(joinPath(projectHome, "Mason.toml"), iomode.r);
    const tomlFile = owned.create(parseToml(toParse));
    if tomlFile.pathExists("brick.license") {
      defaultLicense = tomlFile["brick"]!["license"]!.s;
    }
    // git clone the SPDX repo and validate license identifier
    const dest = MASON_HOME + '/spdx';
    const branch = '--branch master ';
    const depth = '--depth 1 ';
    const url = 'https://github.com/spdx/license-list.git ';
    const command = 'git clone -q ' + branch + depth + url + dest;
    if !isDir(dest) then runCommand(command);
    var licenseList = listdir(MASON_HOME + "/spdx");
    for licenses in licenseList {
      const licenseName: string = licenses.strip('.txt', trailing=true);
      if licenseName == defaultLicense || defaultLicense == 'None' {
        foundValidLicense = true;
        break;
      }
    }
  }
  if foundValidLicense then return (true, defaultLicense);
  else return (false, defaultLicense);
}

/* Attempts to build the package/
 */
private proc attemptToBuild() throws {
  var sub = spawn(['mason','build','--force'], stdout=PIPE);
  sub.wait();
  if sub.exit_status == 1 {
  writeln('(FAILED) Please make sure your package builds');
  }
  else {
    writeln('(PASSED) Package builds successfully.');
  }
}

/* registryPathCheck accepts the path, username, and trueIfLocal in order to check whether the registry that someone
   is trying to publish to is properly set up and has the correct structure.
 */
private proc registryPathCheck(path : string, username : string, trueIfLocal : bool) throws {
  if path == MASON_HOME {
    var forkCheck = usernameCheck(username);
    if forkCheck == 0 {
      writeln('   The mason-registry is forked under username: ' + username + ' (PASSED)');
      return true;
    }
    else {
      writeln('   You must have a fork of the mason-registry to publish a package (FAILED)');
      return false;
    }
  }
  else {
    if trueIfLocal {
      const isLocalGit = exists(path + '/.git');
      const hasBricks = exists(path + '/Bricks/');
      if !isLocalGit {
        writeln('   Registry with path ' + path + ' is not a git repository. (FAILED)');
        writeln("   Local registries must be git repositorys in order to publish");
        return false;
      }
      else if !hasBricks {
        writeln('   The registry with path ' + path + ' does not have proper registry structure (FAILED)');
        writeln("   A registry must have a /Bricks/ directory to be a valid registry");
        return false;
      }
      else {
        writeln('   The local registry with path ' + path + ' is a valid registry to be publish too (PASSED)');
        return true;
      }
    }
    else {
      return true;
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

/* Checks package for examples */
proc exampleCheck(projectHome: string) {
  if isDir(projectHome + '/example') {
    const examples = listdir(projectHome + '/example');
    return examples.size > 0;
  } else return false;
}

/* Checks package for tests */
proc testCheck(projectHome: string) {
  if isDir(projectHome + '/test') {
    const tests = listdir(projectHome + '/test');
    return tests.size > 0;
  } else return false;
}
/* Returns the mason env
 */
private proc returnMasonEnv() {
  const fakeArgs = ['mason', 'env'];
  masonEnv(fakeArgs);
}

private proc falseIfRemotePath() {
  var registryInEnv = MASON_REGISTRY;
  for (name, registry) in registryInEnv {
    if registry.find(':') != -1 {
      return false;
    }
  }
  return true;
}

/* git tag version formatting */
proc gitTagVersionCheck(projectHome: string) throws {
  var tags = runCommand("git tag -l", true);
  var allTags = tags.split("\n");
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  const tomlFile = owned.create(parseToml(toParse));
  var version = "v" + tomlFile["brick"]!["version"]!.s;
  for tag in allTags {
    if tag == version {
      return (true, allTags, version);
    }
  }
  return (false, allTags, version);
}

/* make sure directory created is same as that of package 
   name in manifest file */
proc namespaceCollisionCheck(projectHome: string) throws {
  var directoryName = basename(projectHome);
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  const tomlFile = owned.create(parseToml(toParse));
  var packageName = tomlFile["brick"]!["name"]!.s;
  if packageName == directoryName then return true;
  else return false;
}

/* Mason toml file formatting */
proc masonTomlFileCheck(projectHome: string) {
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  const tomlFile = owned.create(parseToml(toParse));
  var missingFields : list(string);
  var name, chplVersion, version, source, author, license = false;
  if tomlFile.pathExists("brick.name") then name = true; else missingFields.append('name');
  if tomlFile.pathExists("brick.version") then version = true; else missingFields.append('version');
  if tomlFile.pathExists("brick.chplVersion") then chplVersion = true; else missingFields.append('chplVersion');
  if tomlFile.pathExists("brick.source") then source = true; else missingFields.append('source');
  if tomlFile.pathExists("brick.license") then license = true; else missingFields.append('license');
  if tomlFile.pathExists("brick.authors") then author = true; else missingFields.append('authors');
  if name && version && chplVersion && source
    && author && license then return (true, missingFields);
  else return (false, missingFields);
}

use MasonUtils;
use Spawn;
use FileSystem;
use TOML;
use MasonEnv;
use MasonNew;
use MasonModify;
use Random;




proc masonPublish(args : [] string) throws {
  var dry = false;
  var registry = false;
  var path = MASON_HOME;
  var username = "";
  var trueIfLocal = isPathLocal(path);

  try! {
    if hasOptions(args, "-h", "--help") {
      masonPublishHelp();
      exit(0);
    }
    for arg in args [1..] {
      if arg == "--dry-run" then dry = true;
      else if arg == "--registry" {
        registry = true;
      }
      else {
        path = arg;
      }
    }

    if registry && !path.isEmpty() && !dry {
      if checkPath(path, trueIfLocal) {
        if !trueIfLocal {
          username = getUsername();
          publishPackage(username, path, dry, registry, trueIfLocal);
        }
        else {
          publishPackage("", path, dry, registry, trueIfLocal);
        }
      }
      else {
        throw new owned MasonError(path + " is not a valid path");
      }
    }
    else if args.size == 2 || (!trueIfLocal && registry) && !dry {
      if doesGitOriginExist() {
        username = getUsername();
        var checkResult = usernameCheck(username);
        if checkResult == 0 then publishPackage(username, MASON_HOME, dry, registry, trueIfLocal);
        else {
          const badUsernameError = 'mason-registry is not forked or ' + username + ' is not a valid username \n';
          const badForkError = 'Make sure you are connected to the internet before publishing.';
          throw new owned MasonError(badUsernameError + badForkError);
        }
      }
      else throw new owned MasonError('Must have remote origin for package in order to publish.');
    }
    else if dry {
      dryRun(path, registry, trueIfLocal);
    }
    else {
      var passed = "";
      for arg in args {
        passed += arg + ' ';
      }
      writeln(passed + ' does not meet "mason publish [options] <path>" syntax');
    }
  }
  catch e : MasonError {
    writeln(e.message());
    exit(1);
  }
}


/* Main Script that goes through the act of publishing the package to the mason registry.
   Takes the package owners GitHub username as input will throw errors through command
   line git commands if any of the git calls fails.
 */
proc publishPackage(username: string, path : string, dry : bool, registry : bool, trueIfLocal : bool) throws {
  const packageLocation = absPath(here.cwd());
  var stream = makeRandomStream(int);
  var uniqueDir = stream.getNext(): string;
  const name = getPackageName();
  var safeDir = '';

  if (registry && trueIfLocal) then safeDir = path;
  else {
    safeDir = MASON_HOME + '/tmp/' + name + '-' + uniqueDir;
  }
  try! {
    if !exists(MASON_HOME + '/tmp') then mkdir(MASON_HOME + '/tmp');
    mkdir(safeDir);

    if !registry || (registry && !trueIfLocal) {
      cloneMasonReg(username, safeDir);
      branchMasonReg(username, name, safeDir);
    }

    addPackageToBricks(packageLocation, safeDir, name);
    if (registry && !trueIfLocal) || !registry {
      gitC(safeDir + "/mason-registry", "git add .");
      gitC(safeDir + "/mason-registry", "git commit -m '" + name + "'");
      gitC(safeDir + "/mason-registry", 'git push --set-upstream origin ' + name, true);
      rmTree(safeDir + '/');
      writeln('--------------------------------------------------------------------');
      writeln('Go to the above link to open up a Pull Request to the mason-registry');
    }
    else {
      writeln("Package has been added to local Bricks");
    }
  }
  catch {
    if exists(safeDir) then rmTree(safeDir + '/');
    writeln('Error publishing your package to the mason-registry');
  }
}

/* If --dry-run is passed then it takes the username and checks to see if the mason-registry is forked
 and the package has a git remote origin. If both exist then the package can be published.
 */
proc dryRun(username: string, registry : bool, trueIfLocal : bool) throws {
  if !registry {
    var fork = false;
    var remoteCheck = checkIfForkExists(username: string);
    if remoteCheck == 0 {
      fork = true;
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
    if trueIfLocal {
      writeln("Package can be added to local registry");
    }
    else {
      if doesGitOriginExist() {
        writeln("Package can be published to personal remote registry");
      }
      else throw new owned MasonError('Package does not have a git origin');
    }
  }
}


proc checkPath(path : string, trueIfLocal : bool) throws {
  try! {
    if trueIfLocal {
      if exists(path) && exists(path + "/Bricks/") {
        return true;
      }
      else {
        throw new owned MasonError(path + " is not a valid path");
        exit(0);
      }
    }
    else {
      var command = ('git ls-remote ' + path).split();
      var checkRemote = spawn(command, stdout=PIPE);
      checkRemote.wait();
      if checkRemote.exit_status == 0 then return true;
      else {
        throw new owned MasonError(path + " is not a valid remote path");
        exit(0);
      }
    }
  }
  catch e : MasonError {
    writeln(e.message());
    exit(0);
  }
}

proc isPathLocal(path : string) throws {
  if path.find(":") == 0 {
    return false;
  }
  else return true;
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
proc cloneMasonReg(username: string, safeDir : string) throws {
  try! {
    const gitClone = 'git clone --quiet git@github.com:';
    var ret = gitC(safeDir, gitClone  + username + "/mason-registry mason-registry", false);
    return ret;
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
proc branchMasonReg(username: string, name: string, safeDir: string) throws {
  try! {
    const branchCommand = "git checkout --quiet -b  "+ name: string;
    var ret = gitC(safeDir + '/mason-registry', branchCommand, true);
    return ret;
  }
  catch {
    rmTree(safeDir);
    writeln('Error branching mason-registry');
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
private proc addPackageToBricks(projectLocal: string, safeDir: string, name : string) throws {
  try! {

    const toParse = open(projectLocal+ "/Mason.toml", iomode.r);
    const url = gitUrl();
    var tomlFile = new owned(parseToml(toParse));
    const versionNum = tomlFile['brick']['version'].s;
    mkdir(safeDir + "/mason-registry/Bricks/" + name);
    const baseToml = tomlFile;
    var newToml = open(safeDir + "/mason-registry/Bricks/" + name + "/" + versionNum + ".toml", iomode.cw);
    var tomlWriter = newToml.writer();
    baseToml["brick"]["source"] = url[1..url.length-1];
    tomlWriter.write(baseToml);
    tomlWriter.close();
    }
  catch {
    writeln('ERROR: ' + name + ' already exists in the Bricks');
    rmTree(safeDir + '/');
    exit(1);
  }
}

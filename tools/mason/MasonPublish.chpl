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

/* Top Level procedure that gets called from mason.chpl that takes in arguments from command line
Retruns the help output in '-h' or '--help' exits in the arguements.
If --dry-run is passed then it checks to see if the package is able to be published.
Takes in the username of the package owner as an argument*/
proc masonPublish(args: [] string) throws {
  try! {
    if args.size < 3 {
      masonPublishHelp();
      exit(0);
    }
    var dry = false;
    var username = "";
    if args.size == 3 || args.size == 4 {
      for arg in args[2..] {
        if arg == '-h' || arg == '--help'{
          masonPublishHelp();
          exit(0);
        }
        else if arg == "--dry-run" {
          dry = true;
        }
        else {
          username = arg;
        }
      }
      if doesGitOriginExist() {
        if dry && args.size == 4 {
          dryRun(username);
        }
        else if !dry && args.size == 4 {
          writeln('"' + args[0]+ ' '  + args[1] + ' ' + args[2] + ' ' + args [3] +'" does not meet the "mason publish [options] <username>" syntax');
          writeln('See "mason publish -h" for more details');
          exit(0);
        }
        else if args.size == 3 && !dry {
          var usernameCheck = ('git ls-remote https://github.com/' + username + '/mason-registry').split();                            
          var p = spawn(usernameCheck);
          p.wait();
          if p.exit_status == 0 {
            publishPackage(username);
          }
          else {
                throw new owned MasonError(username + ' is not a valid GitHub username or mason-registry is not forked');
          }
        }
        else if args.size == 3 && dry {
          throw new owned MasonError('Must pass your username with --dry-run');
        }
      }
      else {
        throw new owned MasonError('Must have remote origin set up in repository in order to publish.');
      }
    }
    else throw new owned MasonError('Usage must meet the form "mason publish [options] <username>"');
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* Main Script that goes through the act of publishing the package to the mason registry.
 Takes the package owners GitHub usrename as input will throw errors through command
line git commands if any of the git calls fails.*/
proc publishPackage(username: string) throws {
  try! {
    const packageLocation = getEnv('PWD');
    here.chdir(MASON_HOME);
    const dir =  getEnv('PWD');
    mkdir('tmp');
    here.chdir(MASON_HOME + '/tmp');
    cloneMasonReg(username);
    here.chdir(packageLocation);
    const name = getName();
    branchMasonReg(username, name);
    const cwd = getEnv("PWD");
    const package=  addPackageToBricks(packageLocation);
    here.chdir(MASON_HOME + "/tmp/mason-registry");
    const projectHome = getProjectHome(cwd);
    runCommand("git add .");
    runCommand("git commit -m '" + package +"'");
    runCommand('git push --set-upstream origin ' + package, true);
    here.chdir(MASON_HOME);
    rmTree('tmp');
    writeln('--------------------------------------------------------------------');
    writeln('Go to the above link to open up a Pull Request to the mason-registry');
  }
  catch {
    here.chdir(MASON_HOME);
    rmTree('/tmp/');
    writeln('Error publishing your package to the mason-registry');
  }
}

/* If --dry-run is passed then it takes the username and checks to see if the mason-registry is forked
 and the package has a git remote origin. If both exist then the package can be published. */
proc dryRun(username: string) throws {
  var fork = false;
  var checkIfForkExists = ('git ls-remote https://github.com/' + username + '/mason-registry').split();
  var p = spawn(checkIfForkExists, stdout=PIPE);
  p.wait();
  if p.exit_status == 0 {
    fork = true;
  }
  var git = false;
  if doesGitOriginExist() {
    git = true;
  }
  if git && fork {
    writeln('Package can be published to the mason-registry');
    writeln('Commands that will ran are:');
    writeln('git clone git:github.com:[username]/mason-registry mason-registry');
    writeln('git checkout -b [package name]');
    writeln('Package Name will be added to the Bricks in the mason-registry');
    writeln('git add .');
    writeln('git commit -m [package name]');
    writeln('git push --set-upstream origin [package name]');
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


proc moveToMasonHome() {
  const masonHome = getEnv("MASON_HOME");
  here.chdir(masonHome);
}

/* Clones the mason registery fork from the users repo. Takes username as input. */
proc cloneMasonReg(username: string) throws {
  try! {
    var ret = runCommand("git clone git@github.com:" + username + "/mason-registry mason-registry", true);
    return ret;
    }
  catch {
    writeln('Error cloning the fork of mason-registry');
    writeln('Make sure you have forked the mason-registry on GitHub');
    exit(1);
    }
}

/* Checks to see if 'git config --get remote.origin.url' exists */
proc doesGitOriginExist() {
  var urlExists = runCommand("git config --get remote.origin.url", true);
  if urlExists != '' {
    return true;
  }
  else {
    return false;
  }
}

/*Procedure that reutns the url of the git remote origin */
proc gitUrl() {
  var url = runCommand("git config --get remote.origin.url", true);
  return url;
}

/* Takes the git username and creates a new branch of the mason registry users fork,
 name or branch is taken from the Mason.toml of the mason package.*/
proc branchMasonReg(username: string, name: string) throws {
  try! {
    const masonReg = "/mason-registry/";
    const branchCommand = "git checkout -b "+ name: string;
    var ret = gitC(MASON_HOME + '/tmp/mason-registry', branchCommand, true);
    return ret;
    }
  catch {
    here.chdir(MASON_HOME);
    rmTree('/tmp/');
    writeln('Error branching mason-registry');
    exit(1);
    }
}

/* Gets name from the Mason.toml */
proc getName() throws {
  try! {
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", iomode.r);
    var tomlFile = new owned(parseToml(toParse));
    const name = tomlFile['brick']['name'].s;
    return name;
    }
  catch {
    here.chdir(MASON_HOME);
    rmTree('/tmp');
    writeln('Error getting the name of your package, ensure your package is a mason project');
    exit(1);
    }
}

/* Adds package to the Bricks of the mason-registry branch and then vrares the version.toml
 with the source url of the package's GitHub repo.*/
proc addPackageToBricks(projectLocal: string) : string {
  const url = gitUrl();
  const cwd = getEnv("PWD");
  const toParse = open(projectLocal+ "/Mason.toml", iomode.r);
  var tomlFile = new owned(parseToml(toParse));
  const packageName = tomlFile['brick']['name'].s;
  const versionNum = tomlFile['brick']['version'].s;
  const oldDir = here.cwd();
  here.chdir(MASON_HOME + "/tmp/mason-registry/Bricks/");
  mkdir(packageName);
  here.chdir(MASON_HOME + "/tmp/mason-registry/Bricks/" + packageName + "/");
  const baseToml = tomlFile;
  var newToml = open(versionNum + ".toml", iomode.cw);
  var tomlWriter = newToml.writer();
  baseToml["brick"]["source"] = url(1..url.length-1);
  tomlWriter.write(baseToml);
  tomlWriter.close();
  return packageName;
 }

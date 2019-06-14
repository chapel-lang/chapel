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


proc masonPublish(args) throws {
  try! {
    if args.size == 2 {
      masonPublishHelp();
      exit(0);
    }
    var dry = false;
    var username = "";
    if args.size > 2 {
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
        if dry {
          dryRun(username);
        }
        else {
          publishPackage(username);
        }
      }
      else {
            throw new owned MasonError('Must have package set up as git repo to publish');
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}


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


proc publishPackage(username: string) throws {
  forkMasonReg(username);
  branchMasonReg(username);
  const cwd = getEnv("PWD");
  const package=  addPackageToBricks();
  here.chdir(cwd + "/mason-registry");
  const name = getName();
  const url = gitUrl();
  const projectHome = getProjectHome(cwd);
  here.chdir(cwd +"/mason-registry/");
  runCommand("git add .");
  runCommand("git commit -m '" + package +"'");
  runCommand('git push --set-upstream origin ' + package, true);
  runCommand('git remote add upstream https://github.com/chapel-lang/mason-registry');
  runCommand('git request-pull upstream/master https://github.com/chapel-lang/mason-registry ', true);
  here.chdir(cwd);
  rmTree('mason-registry');
}


proc forkMasonReg(username: string) {
  var ret = runCommand("git clone https://github.com/" + username + "/mason-registry mason-registry", true);
  return ret;
}


proc doesGitOriginExist() {
  var urlExists = runCommand("git config --get remote.origin.url", true);
  if urlExists != '' {
    return true;
  }
  else {
    return false;
  }
}


proc gitUrl() {
  var url = runCommand("git config --get remote.origin.url", true);
  return url;
}


proc branchMasonReg(username: string) {
  const name = getName();
  const localEnv = getEnv("PWD");
  const projectName = getProjectHome(localEnv);
  const Env = localEnv(1..localEnv.length);
  const parsePackageName = open(projectName + "/Mason.toml", iomode.r);
  const masonReg = Env + "/mason-registry/";
  const branchCommand = "git checkout -b "+ name: string;
  var ret = gitC(masonReg, branchCommand, true);
  return ret;
}


proc getName() {
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd);
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  var tomlFile = new owned(parseToml(toParse));
  const name = tomlFile['brick']['name'].s;
  return name;
}


proc addPackageToBricks() : string {
  const url = gitUrl();
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd);
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  var tomlFile = new owned(parseToml(toParse));
  const packageName = tomlFile['brick']['name'].s;
  const versionNum = tomlFile['brick']['version'].s;
  const oldDir = here.cwd();
  here.chdir(oldDir + "/mason-registry/Bricks/");
  mkdir(packageName);
  here.chdir(oldDir + "/mason-registry/Bricks/" + packageName + "/");
  const baseToml = tomlFile;
  var newToml = open(versionNum + ".toml", iomode.cw);
  var tomlWriter = newToml.writer();
  baseToml["brick"]["source"] = url(1..url.length-1);
  tomlWriter.write(baseToml);
  tomlWriter.close();
  return packageName;
 }

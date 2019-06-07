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
    if args.size > 2 {
      for arg in args[2..] {
        if arg == '-h' || arg == '--help'{
          masonPublishHelp();
          exit(0);
        }
      }
    }
    else {
      if isGitExist() {
        forkMasonReg();
        branchMasonReg();
        var package = addPackageToBricks();
        pullRequest(package);
      }
      else throw new owned MasonError("Must have package set up as git repo to publish");
      }
    }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}


proc forkMasonReg(){
  var ret = runCommand("git clone https://github.com/oplambeck/mason-registry mason-registry", true);
  return ret;
}


proc isGitExist() {
  var urlexist = runCommand("git config --get remote.origin.url");
  if urlexist != ''{
    return true;
  }
  else return false;
}


proc geturl() {
  var url = runCommand("git config --get remote.origin.url",true);
  return url;
}


proc branchMasonReg(){
  var name = getName();
  const localEnv = getEnv("PWD");
  const projectname = getProjectHome(localEnv);
  var Env = localEnv(1..localEnv.length);
  const parsePackageName = open(projectname + "/Mason.toml", iomode.r);
  const masonreg = Env + "/mason-registry/";
  const branchCommand = "git checkout -b "+ name: string;
  var ret = gitC(masonreg, branchCommand, true);
  return ret;
}


proc getName(){
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd);
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  var tomlFile = new owned(parseToml(toParse));
  const name = tomlFile['brick']['name'].s;
  return name;
}


proc addPackageToBricks() : string{
  var url = geturl();

  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd);
  const toParse = open(projectHome + "/Mason.toml", iomode.r);
  var tomlFile = new owned(parseToml(toParse));
  const packageName = tomlFile['brick']['name'].s;
  const versionNum = tomlFile['brick']['version'].s;
  const oldDir = here.cwd();
  here.chdir(oldDir + "/mason-registry/Bricks/");
  runCommand("mkdir " + packageName, true);
  here.chdir(oldDir + "/mason-registry/Bricks/" + packageName + "/");
  const baseToml = tomlFile;
  var newToml = open(versionNum + ".toml", iomode.cw);
  var tomlWriter = newToml.writer();
  baseToml["brick"]["source"] = url(1..url.length-1);
  tomlWriter.write(baseToml);
  tomlWriter.close();
  return packageName;
 }


proc pullRequest(package) {
  var name = getName();
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd);
  here.chdir(cwd + "/mason-registry/Bricks/");
  runCommand("git add " + package, true);
  runCommand('git commit -m' + package, true);
  here.chdir(cwd +"/mason-registry/");
  runCommand('git push --set-upstream origin ' + name, true);
  runCommand('git push', true);
  runCommand('git request-pull origin/master https://github.com/oplambeck/mason-registry '+name, true);
  here.chdir(cwd);
  runCommand('rm -rf mason-registry');
  writeln();
  writeln('----------------------------------------------------');
  writeln(geturl());
  writeln('Package has been added to a branch of mason-registry');
  writeln('Pull request to be added to master branch of mason-registry has been initiated');
}
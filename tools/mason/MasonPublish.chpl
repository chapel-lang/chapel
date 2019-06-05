/*
*Design Plan
*First fork the mason-registry repo
*branch the repo and add the mason.toml into Bricks/<package-name>/<version>.toml
*add source field to <version>.toml that points to the repo of the package
*pull request from mason-registry from branch that contaons the <version>/toml
*need to figure out how the environment of the package where mason is invoked passes its env to mason 
*
*
*/

use MasonUtils;
use Spawn;
use FileSystem;
use TOML;
use MasonEnv;
use MasonNew;
use MasonModify;

proc masonPublish(args) throws {
  /* Implement a try catch to catch the errors from functions within this function*/
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
        /* throw new MasonError("Must have package set up as git repo to publ;ish...... */
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

proc pullRequest(package)
{
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
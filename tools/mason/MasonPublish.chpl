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

proc masonPublish(){
  forkMasonReg();
  branchMasonReg();
  var package = addPackageToBricks();
  pullRequest(package);
}


proc forkMasonReg(){
  var usernameurl = geturl();
  var tail = usernameurl.find("/")-1: int;
  var head = usernameurl.find(":")+1: int;
  var username = usernameurl(head..tail);
  var ret = runCommand("git clone https://github.com/oplambeck/mason-registry mason-registry", true);
  return ret;
}

proc geturl(){
  var url = runCommand("git config --get remote.origin.url", true);
  return url;
}

proc branchMasonReg(){
  var localEnv = runCommand("pwd", true):string;
  var Env = localEnv(1..localEnv.length-1);
  const masonreg = Env + "/mason-registry/";
  const branchCommand = "git checkout -b newPackageRequest": string;
  var ret = gitC(masonreg, branchCommand, true);
  return ret;
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
  const cwd = getEnv("PWD");
  const projectHome = getProjectHome(cwd);
  here.chdir(cwd + "/mason-registry/Bricks/");
  runCommand("git add " + package, true);
  runCommand('git commit -m' + package, true);
  here.chdir(cwd +"/mason-registry/");
  runCommand('git push --set-upstream origin newPackageRequest', true);
  runCommand('git push', true);
  runCommand('git request-pull master https://github.com/oplambeck/mason-registry newPackageRequest', true);
}
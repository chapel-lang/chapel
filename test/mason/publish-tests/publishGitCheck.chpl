use FileSystem;
use MasonPublish;
use MasonUtils;
use Spawn;
use TOML;


proc main(){
  runCommand('mason new testdir',true);
  var pwd = getEnv("PWD");
  here.chdir(pwd + '/testdir');
  if doesGitOriginExist() == false{
    here.chdir(pwd);
    rmTree('testdir');
    writeln("Passed! Should return empty string as no repo was initialized");
    return 0;
  }
  else {
    return 1;
  }
}
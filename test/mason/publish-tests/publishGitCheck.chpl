use FileSystem;
use MasonPublish;
use MasonUtils;
use Spawn;
use TOML;
use MasonNew;

proc main(){
  masonNew(['mason', 'new', 'publishCheck']);
  var pwd = getEnv("PWD");
  here.chdir(pwd + '/publishCheck');
  if doesGitOriginExist() == false {
    here.chdir(pwd);
    rmTree('publishCheck');
    writeln("Passed! Should return empty string as no repo was initialized");
    return 0;
  }
  else {
    return 1;
  }
}
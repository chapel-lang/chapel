use FileSystem;
use MasonNew;
use MasonPublish;

proc main(){
  masonNew(['new', 'publishCheck']);
  var pwd = here.cwd();
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

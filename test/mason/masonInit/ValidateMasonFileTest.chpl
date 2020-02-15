use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const args = ['init', 'testDir'];
  mkdir("testDir");
  masonInit(args);
  runCommand('rm testDir/Mason.toml');
  runCommand('touch testDir/Mason.toml');
  masonInit(args);
  
  runCommand('rm -rf testDir/');
}

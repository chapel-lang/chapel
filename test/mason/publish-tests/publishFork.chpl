use MasonPublish;
use MasonUtils;
use FileSystem;
use Spawn;

proc main() {
  var cmd = 'cloneMasonReg("test", here.cwd())';
  var result = runWithStatus(cmd, show=false);
  if result != 0 then exit(0);
  else {
    writeln('cloneMasonReg did not fail when it was supposed to');
  }
}
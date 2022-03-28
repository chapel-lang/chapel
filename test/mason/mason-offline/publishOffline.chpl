use FileSystem;
use List;
use MasonPublish;


proc main() {
  var args: list(string);
  args.append('publish');
  here.chdir('offlinePackage');
  masonPublish(args);
}
use FileSystem;
use List;
use MasonPublish;


proc main() {
  var args: list(string);
  args.pushBack('publish');
  here.chdir('offlinePackage');
  masonPublish(args);
}

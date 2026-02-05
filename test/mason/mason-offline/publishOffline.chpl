use FileSystem;
use List;
use MasonPublish;


proc main() {
  here.chdir('offlinePackage');
  masonPublish(["publish"]);
}

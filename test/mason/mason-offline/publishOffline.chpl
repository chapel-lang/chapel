use MasonPublish;
private use List;
use FileSystem;
	
	
proc main() {
  var args: list(string);
  args.append('mason');
  args.append('publish');
  here.chdir('offlinePackage');
  masonPublish(args);
} 
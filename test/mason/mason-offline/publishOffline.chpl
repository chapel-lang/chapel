use FileSystem;
use List;
use MasonPublish;
use MasonUtils;


proc main() {
  var args: list(string);
  args.pushBack('publish');
  here.chdir('offlinePackage');
  try {
    masonPublish(args);
  } catch e: MasonError {
      writeln(e.message());
  } catch {
    writeln("An unknown error occurred during publish.");
  }
}

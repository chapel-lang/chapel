use FileSystem;
use List;
use MasonPublish;
use MasonUtils;


proc main() {
  here.chdir('offlinePackage');

  try {
   masonPublish(["publish"]);
  } catch e: MasonError {
      writeln(e.message());
  } catch {
    writeln("An unknown error occurred during publish.");
  }
}

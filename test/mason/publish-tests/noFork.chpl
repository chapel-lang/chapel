
use MasonPublish;
use MasonUtils;


proc badDryRun() throws {
  try! {
    dryRun('this user name has not forked the repo');
  }
  catch e : MasonError {
    writeln(e.message());
    exit(0);
  }
}


badDryRun();

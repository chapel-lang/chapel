
use MasonPublish;
use MasonUtils;


proc badDryRun() throws {
  try! {
    dryRun('DummyGitUserName');
  }
  catch e : MasonError {
    writeln(e.message());
    exit(0);
  }
}


badDryRun();

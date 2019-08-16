
use MasonPublish;
use MasonUtils;


proc badDryRun() throws {
  try! {
    dryRun('DummyGitUserName','DummyPath', true);
  }
  catch e : MasonError {
    writeln(e.message());
    exit(0);
  }
}


badDryRun();

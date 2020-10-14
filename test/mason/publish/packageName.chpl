
use MasonPublish;
use MasonUtils;

proc packageName() throws {
  try! {
    getPackageName();
  }
  catch e : MasonError {
    writeln(e.message());
  }
}

packageName();

use MasonNew;
use MasonUtils;

config const name="";

proc main() throws {
  const args = ['new', name];
  try {
    masonNew(args);
  } catch e: MasonError {
    writeln(e.message());
  }
}

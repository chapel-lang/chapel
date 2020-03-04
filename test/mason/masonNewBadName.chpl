use MasonUtils;
use FileSystem;
use MasonNew;

config const name="";

proc main() {
  const args : [0..2] string;
  args = ['mason','new', name];
  masonNew(args);
}

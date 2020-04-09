use MasonUtils;
use FileSystem;
use MasonNew;

config const name="";

proc main() {
  const args = ['mason','new', name];
  masonNew(args);
}

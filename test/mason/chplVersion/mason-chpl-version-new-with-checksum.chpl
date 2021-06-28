
use MasonUtils;
use MasonNew;
use IO;

proc main() {
  const args : [0..2] string;
  args = ['mason','new', 'newTest'];

  assert(isDir("newTest") == false);

  masonNew(args);

  assert(isDir("newTest") == true);
  const fi = open("newTest/Mason.toml", iomode.r);
  for line in fi.lines() do write(line);
  fi.close();

  rmTree("newTest");
  assert(isDir("newTest") == false);
}

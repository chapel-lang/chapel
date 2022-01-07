use IO;
use MasonNew;
use MasonUtils;


proc main() {
  const args = ["new", "newTest", "--no-checksum"];

  assert(isDir("newTest") == false);

  masonNew(args);

  assert(isDir("newTest") == true);
  const fi = open("newTest/Mason.toml", iomode.r);
  for line in fi.lines() do write(line);
  fi.close();

  rmTree("newTest");
  assert(isDir("newTest") == false);
}

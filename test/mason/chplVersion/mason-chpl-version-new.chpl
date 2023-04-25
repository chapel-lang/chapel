use IO;
use MasonNew;
use MasonUtils;


proc main() {
  const args : [0..1] string;
  args = ['new', 'newTest'];

  assert(isDir("newTest") == false);

  masonNew(args);

  assert(isDir("newTest") == true);
  const fr = openReader("newTest/Mason.toml");
  for line in fr.lines() do write(line);
  fr.close();

  rmTree("newTest");
  assert(isDir("newTest") == false);
}

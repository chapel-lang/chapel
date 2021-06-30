use IO;
use Path;

var f = open("foo", iomode.cw);

assert(f.getParentName() == dirname(realPath(f)));
assert(f.absPath() == absPath(f));
assert(f.realPath() == realPath(f));
assert(f.relPath() == relPath(f));

f.close();

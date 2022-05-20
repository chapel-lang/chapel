use IO;
use FileSystem;
use Path;
use BlockDist;
use OS.POSIX;

config const verbose = false;

var files = ["open-remote-string-bar", "open-remote-string-baz"];

var Space = files.domain dmapped Block(files.domain);
var DistFiles:[Space] string = files;

for f in files {
  var openf = open(f, iomode.cw);
  openf.close();
}

for f in DistFiles {
  on f {
    var from = f;
    var base = basename(f);
    const uname = getenv(c"USER"):c_string;
    var to = "/tmp/" + createStringWithNewBuffer(uname)+ base;
    if verbose then writeln("on ", here.id, " copying from ", from, " to ", to);
    copy(from, to);
    f = to;
  }
}

for f in DistFiles {
  on f {
    if verbose then writeln("on ", here.id, " opening ", f);
    var openf = open(f, iomode.r);
    if verbose then writeln("on ", here.id, " done opening ", f);
  }
}

for f in DistFiles {
  on f {
    if verbose then writeln("on ", here.id, " removing ", f);
    remove(f);
  }
}

for f in files {
  remove(f);
}



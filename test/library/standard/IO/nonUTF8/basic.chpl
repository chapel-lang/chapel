use IO;
use ChplConfig;

config param useNonUTF8 = true;

//helper to create \xffstr\xffstr\xff
proc s(str) {
  if useNonUTF8 then
    return b"\xff%s\xff%s\xff".format(str, str).decode(policy=decodePolicy.escape);
  else
    return str;
}

const filename1 = s("junkfile1");

var f = open(filename1, ioMode.cw);
var p = f.path;
writeln("file.path works: ", p == CHPL_HOME + "/test/library/standard/IO/nonUTF8/" + filename1);
f.close();

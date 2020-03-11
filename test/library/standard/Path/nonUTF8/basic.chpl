use FileSystem;
use Path;

proc repr(str) {
  writef("%ht\n", str.encode(policy=encodePolicy.unescape));
}

const dirName = b"\xffNOT\xffUTF8\xff".decode(policy=decodePolicy.escape);

mkdir(dirName);
const dirPath = realPath(dirName);

here.chdir(dirName);
const cur = here.cwd();
const arg = cur+"/$THERES_NO_SPOON";

repr(realPath(cur));
repr(expandVars(arg));
rmTree(dirPath);

use FileSystem;
use Path;

proc repr(str) {
  writef("%ht\n", str.encode(errors=encodePolicy.unescape));
}

const dirName = b"\xffNOT\xffUTF8\xff".decode(errors=decodePolicy.escape);

mkdir(dirName);
const dirPath = realPath(dirName);

here.chdir(dirName);
const cur = here.cwd();
const arg = cur+"/$THERES_NO_SPOON";

repr(realPath(cur));
repr(expandVars(arg));
rmTree(dirPath);

use FileSystem;
use Path;

proc repr(str) {
  writef("%ht\n", str.encode(errors=encodePolicy.pass));
}

/*const dirName = b"\xffNOT\xffUTF8\xff";*/
const dirName = b"asd";

/*const junk = "\xffASD\xff";*/
/*writef("%ht\n", junk);*/

mkdir(dirName.decode(errors=decodePolicy.escape));
const dirPath = realPath(dirName.decode());

here.chdir(dirName.decode(errors=decodePolicy.escape));
const cur = here.cwd();
const arg = cur+"/$THERES_NO_SPOON";

repr(realPath(cur));
repr(expandVars(arg));
rmTree(dirPath);
/*rmTree(dirName.decode(errors=decodePolicy.escape));*/

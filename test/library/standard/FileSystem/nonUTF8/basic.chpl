use FileSystem;
use IO;
use Sort;
use List;

config param useNonUTF8 = true;

//helper to create \xffstr\xffstr\xff
proc s(str) {
  if useNonUTF8 then
    return b"\xff%s\xff%s\xff".format(str, str).decode(errors=decodePolicy.escape);
  else
    return str;
}

const startDir = here.cwd();

const dirname1 = s("junkbase");
const filename1 = s("junkfile1");
const filename2 = s("junkfile2");
const linkname1 = s("junklink1");

// prep
if exists(dirname1) {
  rmTree(dirname1);
}


writeln("Creating a directory");
mkdir(dirname1);
writeln("isFile works: ", isFile(dirname1) == false);
writeln("isDir works: ", isDir(dirname1) == true);
writeln("isLink works: ", isLink(dirname1) == false);
writeln("isMount works: ", isMount(dirname1) == false);
writeln("exists works: ", exists(dirname1) == true);
writeln();

writeln("Changing directory");
here.chdir(dirname1);

writeln("Creating file");
var f = open(filename1, iomode.cw);
var writer = f.writer();
writer.write("test file");
writer.close();
f.close();
writeln("isFile works: ", isFile(filename1) == true);
writeln("isDir works: ", isDir(filename1) == false);
writeln("isLink works: ", isLink(filename1) == false);
writeln("isMount works: ", isMount(filename1) == false);
writeln("exists works: ", exists(filename1) == true);

const gid = getGID(filename1);
const uid = getUID(filename1);
const mode = getMode(filename1);
const size = getFileSize(filename1);
writeln();

writeln("Creating a copy");
copy(filename1, filename2);
writeln("getGID works: ", getGID(filename2) == gid);
writeln("getUID works: ", getUID(filename2) == uid);
writeln("getMode works: ", getMode(filename2) == mode);
writeln("getFileSize works: ", getFileSize(filename2) == size);
writeln();

writeln("Attempting to chown");
try! {
  // chown and back -- in some systems this runs successfully
  chown(filename2, uid+1, gid+1);
  chown(filename2, uid, gid);
  writeln("chown works");
}
catch e: PermissionError {
  writeln("chown works");
}
writeln();

writeln("chmod'ing the file");
chmod(filename2, 644);
writeln("chmod works: ", getMode(filename2) == 644);
chmod(filename2, mode); // change it back
writeln();


writeln("Creating a symlink");
symlink(filename1, linkname1);
writeln("isFile works: ", isFile(linkname1) == true);
writeln("isDir works: ", isDir(linkname1) == false);
writeln("isLink works: ", isLink(linkname1) == true);
writeln("isMount works: ", isMount(linkname1) == false);
writeln("exists works: ", exists(linkname1) == true);
writeln();

writeln("Removing the symlink");
remove(linkname1);
writeln("exists works: ", exists(linkname1) == false);
writeln("exists works: ", exists(filename1) == true);
writeln();


writeln("Going back to the initial dir");
here.chdir(startDir);
writeln();


writeln("listing the dir contents");
// the order seems to change from system to system
var l: list(string);
for f in listdir(dirname1) {
  l.append(f);
}
for f in sorted(l.toArray()) {
  writef("%ht\n", f.encode(errors=encodePolicy.unescape));
}
writeln();

writeln("Removing the remaining junk");
rmTree(dirname1);
writeln("exists works: ", exists(dirname1) == false);
writeln();

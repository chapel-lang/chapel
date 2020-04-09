use FileSystem;
use Path;

// Test that removal of a directory tree which contains a symlink to a
// directory does not remove the directory which was linked to, and does
// remove the symlink itself.

var linkDir = "hasLink2";
var link = "hasLink2/amALink";
var linkedSrc = "linksSrc2/inner";
var linkedContents = "linksSrc2/inner/depths.txt";
if (!exists(linkDir)) then mkdir(linkDir);

if (exists(linkDir) && !isDir(linkDir)) {
  writeln("Removing hasLink as it is not a directory and we need a directory");
  remove(linkDir);
  mkdir(linkDir);
}

if (exists(link)) {
  writeln("Something using the same name as our link was in this directory.");
  writeln("Awkward... *deletes*");
  remove(link);
}

writeln("Creating a symlink");
symlink(realPath(linkedSrc), link);
// Need to provide full path to symlink.  Is this a bug?  Matches behavior of
// Linux's ln -s call.
writeln(linkDir, " exists: ", exists(linkDir));
writeln(link, " exists: ", exists(link));
writeln(linkedSrc, " exists: ", exists(linkedSrc));
writeln(linkedContents, " exists: ", exists(linkedContents));

rmTree(linkDir); // Removal call.
writeln("Post rmTree call");
writeln(linkDir, " exists: ", exists(linkDir));
writeln(link, " exists: ", exists(link));
writeln(linkedSrc, " exists: ", exists(linkedSrc));
writeln(linkedContents, " exists: ", exists(linkedContents));

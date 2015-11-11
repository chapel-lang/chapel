use FileSystem;
use Path;

// This test ensures that copyTree works as expected for copySymbolically =
// true or false when the source directory contains a symlink.
var linkDir = "hasLink";
var link = "hasLink/amALink";
var linkSrc = "linksSrc/linked.txt";

// Test set up
if (!exists(linkDir)) {
  mkdir(linkDir);
}
if (!exists(link)) {
  symlink(realPath(linkSrc), link);
}
if (!isLink(link)) {
  writeln(link + " was not a symlink, removing and replacing");
  remove(link);
  symlink(realPath(linkSrc), link);
}

var destLinked = "willLink";
var destCopied = "willCopy";

copyTree(linkDir, destLinked, true);
// destLinked is expected to contain a symlink to linkSrc
var linkInDest = "willLink/amALink";
writeln("copying symbolically, copy exists: " + exists(linkInDest));
writeln("and is a link: " + isLink(linkInDest));

copyTree(linkDir, destCopied, false);
// destCopied is expected to contain a file with the exact contents of linkSrc
var copyInDest = "willCopy/amALink";
writeln("copying un-symbolically, copy exists: " + exists(copyInDest));
writeln("and is a link: " + isLink(copyInDest));

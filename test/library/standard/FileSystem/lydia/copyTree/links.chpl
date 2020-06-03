use FileSystem;
use Path;

// This test ensures that copyTree works as expected for copySymbolically =
// true or false when the source directory contains a symlink.
var dirWithLinks = "hasLink";
var fileLink = "hasLink/amALink";
var dirLink = "hasLink/dirLink";
var linkSrc = "linksSrc/linked.txt";
var linkSrcDir = "linksSrc";

// Create directory to copy
if (!exists(dirWithLinks)) {
  mkdir(dirWithLinks);
}

// Populate directory to copy with a file link
if (!exists(fileLink)) {
  symlink(realPath(linkSrc), fileLink);
}
if (!isLink(fileLink)) {
  writeln(fileLink + " was not a symlink, removing and replacing");
  remove(fileLink);
  symlink(realPath(linkSrc), fileLink);
}

// Populate directory to copy with a directory link
if (!exists(dirLink)) {
  symlink(realPath(linkSrcDir), dirLink);
}
if (!isLink(dirLink)) {
  writeln(dirLink + " was not a symlink, removing and replacing");
  rmTree(dirLink);
  symlink(realPath(linkSrcDir), dirLink);
}

var destLinked = "willLink";
var destCopied = "willCopy";

copyTree(dirWithLinks, destLinked, true);
// destLinked is expected to contain symlinks to linkSrc
var fileLinkInDest = "willLink/amALink";
var dirLinkInDest = "willLink/dirLink";
writeln("Copying symbolically");
writeln("file copy exists: ", exists(fileLinkInDest));
writeln("and is a link: ", isLink(fileLinkInDest));
writeln("dir copy exists: ", exists(dirLinkInDest));
writeln("and is a link: ", isLink(dirLinkInDest));

copyTree(dirWithLinks, destCopied, false);
// destCopied is expected to contain a file with the exact contents of linkSrc
var fileCopyInDest = "willCopy/amALink";
var dirCopyInDest = "willCopy/dirLink";
writeln("Copying un-symbolically");
writeln("file copy exists: ", exists(fileCopyInDest));
writeln("and is a link: ", isLink(fileCopyInDest));
writeln("dir copy exists: ", exists(dirCopyInDest));
writeln("and is a link: ", isLink(dirCopyInDest));

use FileSystem;
use Path;

// This test ensures that moveDir works as expected when the source directory
// contains a symlink.
var dirWithLinks = "hasLink";
var fileLink = "hasLink/amALink";
var dirLink = "hasLink/dirLink";
var linkSrc = "linksSrc/linked.txt";
var linkSrcDir = "linksSrc";

// Create directory to move
if (!exists(dirWithLinks)) {
  mkdir(dirWithLinks);
}

// Populate directory to move with a file link
if (!exists(fileLink)) {
  symlink(realPath(linkSrc), fileLink);
}
if (!isLink(fileLink)) {
  writeln(fileLink, " was not a symlink, removing and replacing");
  remove(fileLink);
  symlink(realPath(linkSrc), fileLink);
}

// Populate directory to move with a directory link
if (!exists(dirLink)) {
  symlink(realPath(linkSrcDir), dirLink);
}
if (!isLink(dirLink)) {
  writeln(dirLink, " was not a symlink, removing and replacing");
  rmTree(dirLink);
  symlink(realPath(linkSrcDir), dirLink);
}

var destLinked = "linkDest";

moveDir(dirWithLinks, destLinked);
// destLinked is expected to contain symlinks to linkSrc
var fileLinkInDest = "linkDest/amALink";
var dirLinkInDest = "linkDest/dirLink";
writeln("moved file exists: ", exists(fileLinkInDest));
writeln("and is a link: ", isLink(fileLinkInDest));
writeln("moved dir exists: ", exists(dirLinkInDest));
writeln("and is a link: ", isLink(dirLinkInDest));
writeln("Source directory does not exist any more: ", !exists(dirWithLinks));

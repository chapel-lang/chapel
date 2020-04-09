use FileSystem;
use Path;

var srcName = "emptyFolder";
var destName = "destFolder";

if (!exists(srcName)) {
  mkdir(srcName);
} else {
  var srcChildren = listdir(realPath(srcName));
  if (srcChildren.size > 0) {
    writeln("Removing tree at ", srcName, ", does not fulfill test conditions");
    rmTree(srcName);
    mkdir(srcName);
  }
}

if (exists(destName)) {
  rmTree(destName);
}

writeln(srcName, " exists and is empty: ", exists(srcName));
writeln(destName, " exists: ", exists(destName));
moveDir(srcName, destName);
writeln(srcName, " exists: ", exists(srcName));
writeln(destName, " exists: ", exists(destName));
var destChildren = listdir(realPath(destName));
writeln(destName, " is empty: ", (destChildren.size == 0));

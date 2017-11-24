use Path;

/* Tests IO.file.getParentName(syserror) function */
proc main() {
  var f = open('testFile.txt', iomode.r),
      err: syserr;

  writeln(f.getParentName(err));
}

use IO;
var f = open("fileDotReaderLockingDefault.chpl", ioMode.r);

// should trigger a warning, locking isn't specified
var r = f.reader();
write(r.readLine());

// shouldn't trigger a warning, locking is specified explicitly
var r2 = f.reader(locking=false);
write(r2.readLine());

use IO;
var r = openReader("openReaderLockingDefault.chpl");
write(r.readLine());

// this shouldn't produce a warning because locking is set explicitly
var r2 = openReader("openReaderLockingDefault.chpl", locking=false);
write(r2.readLine());

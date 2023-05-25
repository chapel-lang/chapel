use CTypes, OS.POSIX;

config const size : c_size_t = 256 * 1024 * 1024;
config const numTasks = max(here.maxTaskPar, max(int(8)));

config const trials = 3;
coforall i in 1..here.maxTaskPar {
  var taskid = i % max(int(8));
  for 1..trials {
    var m = allocate(int(8), size);
    memset(m, taskid, size.safeCast(c_size_t));
    assert(m[0] == taskid && m[size-1] == taskid);
    deallocate(m);

    var a = allocate(int(8), size, alignment=8);
    memset(a, taskid, size.safeCast(c_size_t));
    assert(a[0] == taskid && a[size-1] == taskid);
    deallocate(a);

    var c = allocate(int(8), size, clear=true);
    assert(c[0] == 0 && c[size-1] == 0);
    deallocate(c);
  }
}

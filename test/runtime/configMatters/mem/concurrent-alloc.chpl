use CPtr;

config const size = 256 * 1024 * 1024;
config const numTasks = max(here.maxTaskPar, max(int(8)));

config const trials = 3;
coforall taskid in 1..here.maxTaskPar {
  for 1..trials {
    var m = c_malloc(int(8), size);
    c_memset(m, taskid, size);
    assert(m[0] == taskid && m[size-1] == taskid);
    c_free(m);

    var a = c_aligned_alloc(int(8), 8, size);
    c_memset(a, taskid, size);
    assert(a[0] == taskid && a[size-1] == taskid);
    c_free(a);

    var c = c_calloc(int(8), size);
    assert(c[0] == 0 && c[size-1] == 0);
    c_free(c);
  }
}

config const numTasks=4;

iter iterate() {
  coforall loc in Locales {
//    on loc {
      coforall taskid in 0..#numTasks {
        const (lo,hi) = (1, 10);
        yield {lo..hi};
      }
//    }
  }
}

for i in iterate() {
  writeln(here.id, " got ", i);
}

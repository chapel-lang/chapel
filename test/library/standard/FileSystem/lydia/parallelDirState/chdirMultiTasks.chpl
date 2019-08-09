use FileSystem;
// This test shows that the current working directory is shared among the tasks
// on a single locale, so a chdir() call in one task will affect the output of
// cwd() in another.
var mylock1: sync int;
var mylock2: sync int;

here.chdir("task1/0");
writeln(here.cwd());
cobegin {
  m1(); // Task 1 only calls here.cwd()
  m2(); // Task 2 only changes the directory
}
writeln(here.cwd()); // The starting task is affected as well


proc m1() {
  for i in 1..5 {
    mylock1.readFE();
    writeln(here.cwd());
    mylock2.writeEF(1);
  }  
}

proc m2() {
  mylock1.writeEF(1);
  for i in 1..5 {
    mylock2.readFE();
    here.chdir("../"+i:string);
    mylock1.writeEF(1);
  }
}

proc dothis(r:func(int, int)):int; 

proc addthree(a:int):int {
  return a+3;
}

proc main() {
  writeln(dothis(addthree));
}

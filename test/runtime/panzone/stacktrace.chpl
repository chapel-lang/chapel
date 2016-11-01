proc a() {
  halt();
}

proc b(){
  a();
}

proc c(){
  b();
}

c();

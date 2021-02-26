config const n: int = 256;

extern proc printf(fmt:c_string, x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", (here.id:string + " pre " + i:string).c_str());
    cobegin {
      foo(i+1);
      ;
    }
    printf("%s\n", (here.id:string + " post " + i:string).c_str());
  }
}

foo(1);

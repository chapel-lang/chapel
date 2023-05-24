use CTypes;
config const n: int = 256;

extern proc printf(fmt:c_string, x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", c_ptrToConst_helper(here.id:string + " pre " + i:string):c_string);
    cobegin {
      foo(i+1);
      ;
    }
    printf("%s\n", c_ptrToConst_helper(here.id:string + " post " + i:string):c_string);
  }
}

foo(1);

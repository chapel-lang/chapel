class C { }

proc error() {
  // These are just never mentioned again
  type t1;
  
  // These have invalid split inits
  type t2;

  writeln(t2:string);

  t2 = int;
}
error();

type tt;

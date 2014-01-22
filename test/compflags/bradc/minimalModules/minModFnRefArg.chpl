extern proc printf(x...);
proc foo(ref x) {
  printf("%ld\n", x);
  x = 2;
}

var x = 1;

foo(x);

printf("%ld\n", x);

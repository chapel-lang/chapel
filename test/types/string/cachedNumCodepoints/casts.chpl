proc test(val) {
  writeln((val:string).cachedNumCodepoints);
}

test(1);
test(1.1);
test(1..1);
test((1,1));

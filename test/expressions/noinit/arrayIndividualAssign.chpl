var arr: [1..5] int = noinit;
for i in arr.domain {
  arr[i] = i;
}
writeln(arr);

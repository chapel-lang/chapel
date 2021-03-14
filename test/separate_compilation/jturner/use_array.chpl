proc dothis(r:[1..2] real): real;

proc main() {
  var arr : [1..2] real;
  arr[1] = 2.0;
  arr[2] = 3.25;
  writeln(dothis(arr));
}

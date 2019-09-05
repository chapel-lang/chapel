proc main() {
  var s : string;
  for i in 1..10 do s += i:string;

  {
    var lines : [1..0] string;

    lines.push_back(s);
    lines.pop_front(); // lines.remove(1) also leaks
  }
}

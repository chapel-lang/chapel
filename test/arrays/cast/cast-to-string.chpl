var a = [i in 1..10] i;

operator :(arr: [], type t:string) {
  var s = "---";
  var first = true;
  for x in arr {
    if first
      then first = false;
      else s += " ";
    s += x:string;
  };
  s += "---";
  return s;
}

writeln(a:string);

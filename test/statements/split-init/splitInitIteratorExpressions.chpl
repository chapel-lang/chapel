var cond = "abc<def";
var catlim;
if cond.find(">") >= 0 {
  catlim = cond.split(">");
} else {
  catlim = cond.split("<");
}

writeln(catlim);

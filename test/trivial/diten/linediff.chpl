
config const s1 = "string 1";
config const s2 = "string 2";

proc main(){
  var temp_str = "";
  var l1 = (s1.size);
  var l2 = (s2.size);
  var minlength : int = min(l1, l2);
  var maxlength : int = max(l1, l2);

  if s1 == s2 then
    return;

  for i in 1..minlength {
    if s1[i] != s2[i] then
      temp_str = temp_str + '|';
    else
      temp_str = temp_str + '-';
  }

  for i in 1..maxlength-minlength {
    temp_str = temp_str + '|';
  }
  writeln(s1);
  writeln(temp_str);
  writeln(s2);

}

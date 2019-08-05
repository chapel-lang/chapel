config const n = 1024*1024;

on Locales[numLocales-1] {
  var arr : [0..-1] int;
  for i in 1..n {
    arr.push_back(i);
  }
}

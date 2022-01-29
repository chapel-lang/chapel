use Shlex;
for src in ['abc def ', 'abc def\n', 'abc def \t\n', 'abc   def \n ', 'abc\ndef\n', 'abc\ndef\n\t'] do {
  var ans=split(src);
  writeln(ans);
}

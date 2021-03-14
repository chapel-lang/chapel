
coforall Locales with (
                       var iii: int
                       ) {
  writeln(iii);
}

cobegin with (
              var jjj: int
              ) {
  writeln(jjj);
  writeln("no jjj");
}

begin with (
            var kkk: int
            ) {
  writeln(kkk);
}

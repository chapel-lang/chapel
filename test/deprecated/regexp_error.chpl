use Regexp;

{
  var re : regexp(string);
  writeln(re.ok);
  writeln("111111111111111111111");
  try! {
    re = compile("*");
    writeln("Should not reach here");
  } catch (e: BadRegexpError) {
    writeln("222222222222222222222");
    writeln(re.ok);
    writeln("333333333333333333333");
    writeln(re.error());
    writeln("444444444444444444444");
    writeln(e.message());
  }
}

{
  writeln("555555555555555555555");
  var re = compile(".");
  writeln(re.ok);
}

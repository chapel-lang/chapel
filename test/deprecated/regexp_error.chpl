use Regex;

{
  var re : regex(string);
  writeln("111111111111111111111");
  try! {
    re = compile("*");
    writeln("Should not reach here");
  } catch (e: BadRegexpError) {
    writeln(e.message());
  }
}


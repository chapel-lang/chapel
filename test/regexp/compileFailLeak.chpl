use Regex;

{
  var re : regex(string);
  writeln("111111111111111111111");
  try! {
    re = compile("*");
    writeln("Should not reach here");
  } catch (e: BadRegexError) {
    writeln("222222222222222222222");
    writeln(e.message());
  }
}

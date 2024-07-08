
use IO;
use FormatHelper;

proc helper(things) {
  stdout.withSerializer(FormatWriter).writeln(things);
}

proc main() {
  helper(for i in 1..10 do i);
}

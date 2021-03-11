use Regexp;                                      // deprecated module
try! {
    var re = Regexp.compile("*");
} catch err : BadRegexpError {                   // deprecated type
    writeln(err.message());
}
var re : regexp = Regexp.compile("a*");          // deprecated type
var match : reMatch = re.match("aaaaaaaaaaa");   // deprecated type
writeln(match.matched);

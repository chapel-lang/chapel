var num = "123456890";
var lower = "abcdefghijklmnopqrstuvwxyz";
var upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var alpha = lower+upper;
var alphaNum = num + alpha;
var whitespace = " \t\n\r";

var hello = "hello, world!";
var titleHello = "Hello, World!";
var titleHello2 = "H Ello, World!";
var nottitleHello = "HEllo, World!";

proc test(s, alt="") {
  on Locales[numLocales-1] {
    if alt != "" {
      writeln(alt, ":");
    } else {
      writeln(s, ":");
    }
    writeln("    isUpper: ", s.isUpper());
    writeln("    isLower: ", s.isLower());
    writeln("    isSpace: ", s.isSpace());
    writeln("    isAlpha: ", s.isAlpha());
    writeln("    isDigit: ", s.isDigit());
    writeln("    isAlnum: ", s.isAlnum());
    writeln("    isPrintable: ", s.isPrintable());
  }
}

proc test_title(s) {
  on Locales[numLocales-1] {
    writeln(s, ":");
    writeln("    isTitle: ", s.isTitle());
  }
}

test(num);
test(lower);
test(upper);
test(alpha);
test(alphaNum);
test(whitespace, "|whitespace|");

test_title(hello);
test_title(titleHello);
test_title(titleHello2);
test_title(nottitleHello);

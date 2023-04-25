import Regex;

var myre = new Regex.regex("hello[0-9]");
if myre.match("hello0") then writeln("Yes!");

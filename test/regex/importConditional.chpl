import Regex;

var myre=Regex.compile("hello[0-9]");
if myre.match("hello0") then writeln("Yes!");

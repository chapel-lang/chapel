var unicodeStr = "Türkçe";
writeln("Base unicode string : ", unicodeStr,
        " (length=", unicodeStr.length, ")");

var valid_utf8: _bytes = "T\xc3\xbcrk\xc3\xa7e";
writeln("Print as bytes: ", valid_utf8);
writeln("Decoded string: ", valid_utf8.decode());
assert(valid_utf8.decode() == unicodeStr);

var invalid_utf8: _bytes = "T\xc3\xbc\xffrk\xc3\xa7e"; // \xff is invalid
const ignoredString = invalid_utf8.decode(errors=DecodePolicy.Ignore);
writeln("String with the ignore policy: ", ignoredString,
        " (length=", ignoredString.length, ")");
const replacedString = invalid_utf8.decode(errors=DecodePolicy.Replace);
writeln("String with the replace policy: ", replacedString,
        " (length=", replacedString.length, ")");
try! {
  const strictString = invalid_utf8.decode(errors=DecodePolicy.Strict);
}
catch e:DecodeError {
  writeln(e);
}

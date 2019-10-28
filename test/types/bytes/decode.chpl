var unicodeStr = "Türkçe";
writeln("Base unicode string : ", unicodeStr,
        " (length=", unicodeStr.numBytes, ")");

var valid_utf8= b"T\xc3\xbcrk\xc3\xa7e";
writeln("Print as bytes: ", valid_utf8);
writeln("Decoded string: ", valid_utf8.decode());
assert(valid_utf8.decode() == unicodeStr);

var invalid_utf8 = b"T\xc3\xbc\xffrk\xc3\xa7e"; // \xff is invalid
const ignoredString = invalid_utf8.decode(errors=decodePolicy.ignore);
writeln("String with the ignore policy: ", ignoredString,
        " (length=", ignoredString.numBytes, ")");
const replacedString = invalid_utf8.decode(errors=decodePolicy.replace);
writeln("String with the replace policy: ", replacedString,
        " (length=", replacedString.numBytes, ")");

// what happens if replacement increases the size of the string?
var allInvalidBytes = b"\xff\xff\xff\xff\xff";  // 5 invalid bytes
writeln(allInvalidBytes.decode(errors=decodePolicy.replace).numBytes); // 10

// what happens if there is a broken byte in an otherwise valid multibyte
// sequence?
var hwair = b"\xF0\x90\x8D\x88"; // UTF-8 encoding for gothic letter "hwair"

//first make sure that this is decoded correctly:
writeln(hwair.decode());

writeln("Hwair tests");
//now create an almost equal one with single flipped bit. All continuation bytes
//must be in form 0b10xxxxxx in UTF-8. However, we flip one bit in the last byte
//to make it 0b11xxxxxx, which is invalid
var almostHwair = b"\xF0\x90\x8D\xC8"; // the last byte is broken

// the decoded string should contain two replacement characters in this
// scenario: (1) for the first three characters, because the first byte said
// that this is a 4-byte sequence, but the last byte is not valid continuation
// which should have been in the form 0b10xxxxxxx
var almostHwairToString = almostHwair.decode(decodePolicy.replace);
writeln(almostHwairToString.numBytes); // number of bytes should be 4
writeln(almostHwairToString.length); // number of bytes should be 4
writeln(almostHwairToString.numCodepoints);
for cp in almostHwairToString.codepoints() do
  writeln(cp);
writeln("End Hwair tests");

//finally make sure that the strict error policy is actually strict
try! {
  const strictString = invalid_utf8.decode(errors=decodePolicy.strict);
}
catch e:DecodeError {
  writeln(e);
}

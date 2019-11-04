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
writeln("Should be 15: ",
        allInvalidBytes.decode(errors=decodePolicy.replace).numBytes);

// what happens if there is a broken byte in an otherwise valid multibyte
// sequence?
var hwair = b"\xF0\x90\x8D\x88"; // UTF-8 encoding for gothic letter "hwair"

//first make sure that this is decoded correctly:
writeln(hwair.decode());

writeln("Hwair tests");
//now create an almost equal one with single flipped bit. All continuation bytes
//must be in form 0b10xxxxxx in UTF-8. However, we flip one bit in the last byte
//to make it 0b11xxxxxx, which is invalid. This one also have some ASCII
//(therefore, UTF-8) bytes following the invalid byte
var almostHwairFlippedBit = b"\xF0\x90\x8D\xC8\x41\x42\x43"; // the 4th byte is broken

// the decoded string should contain two replacement characters in this
// scenario: (1) for the first three characters, because the first byte said
// that this is a 4-byte sequence, but the last byte is not valid continuation
// which should have been in the form 0b10xxxxxxx (2) for the last invalid byte
// itself
var almostHwairToStringRepl = almostHwairFlippedBit.decode(decodePolicy.replace);
// number of bytes should be 9 = 2*3 from replacement bytes, 3 from the trailing
// valid bytes
writeln("Should be 9: ", almostHwairToStringRepl.numBytes);

//make sure the decoded buffer is actually valid (i.e. can be re-decoded with
//decodePolicy.strict)
writeln((almostHwairToStringRepl:bytes).decode());


//now create another almost equal one where the fourth byte is replaced with a
//valid ASCII character. All continuation bytes must be in form 0b10xxxxxx in
//UTF-8. So, that ASCII character will break the previous sequence but itself is
//valid and should remain in the decoded string. This one also have some ASCII
//(therefore, UTF-8) bytes following the invalid byte
var almostHwairValidAscii = b"\xF0\x90\x8D\x41\x41\x42\x43"; // the 4th byte is broken

almostHwairToStringRepl = almostHwairValidAscii.decode(decodePolicy.replace);
// number of bytes should be 7 = 1*3 from replacement byte, 4 from the trailing
// valid bytes
writeln("Should be 7: ", almostHwairToStringRepl.numBytes);

//make sure the decoded buffer is actually valid (i.e. can be re-decoded with
//decodePolicy.strict)
writeln((almostHwairToStringRepl:bytes).decode());

var almostHwairToStringIgnr = almostHwairValidAscii.decode(decodePolicy.ignore);
// number of bytes should be 4 -- only the last four must be in the string
writeln("Should be 4: ", almostHwairToStringIgnr.numBytes);

//make sure the decoded buffer is actually valid (i.e. can be re-decoded with
//decodePolicy.strict)
writeln((almostHwairToStringIgnr:bytes).decode());
writeln("End Hwair tests");

//finally make sure that the strict error policy is actually strict
try! {
  const strictString = invalid_utf8.decode(errors=decodePolicy.strict);
}
catch e:DecodeError {
  writeln(e);
}

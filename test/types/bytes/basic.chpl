var b: bytes = "this is a bytes";
var s: string = "this is a string";
var cs: c_string = "this is a c_string";

writeln(b);
writeln(s);
writeln(cs:string);
writeln();

// TEST INITIALIZERS
writeln("Initializer tests");
var b_from_s = new bytes(s);
var b_from_cs = new bytes(cs);

var c_char_arr = c_malloc(uint(8), 4);
c_char_arr[0] = 65; //A
c_char_arr[1] = 66; //B
c_char_arr[2] = 67; //C
c_char_arr[3] = 0;

//length and size are in bytes
var b_from_c_ptr = new bytes(c_char_arr, length=3, size=4);

writeln("bytes object from string: ", b_from_s);
writeln("bytes object from c_string: ", b_from_cs);
writeln("bytes object from c_ptr: ", b_from_c_ptr);
writeln();

// TEST length and size queries
writeln("Length/size tests");
writeln(b_from_s.length, " must be ", s.length);
writeln(b_from_cs.length, " must be ", cs.length);
writeln(b_from_c_ptr.length, " must be ", 3);
writeln(b_from_s.size, " must be ", s.size);
writeln(b_from_cs.size, " must be ", cs.size);
writeln(b_from_c_ptr.size, " must be ", 3);
writeln();

// TODO test localize in a multilocale setup

// c_str is tested with writeln anyways

// TEST ACCESSORS
writeln("Accessor tests");
for i in 1..b_from_c_ptr.length do
  writeln(b_from_c_ptr[i], " as ", b_from_c_ptr[i].type:string);
writeln();

//TEST ITERATORS
writeln("Iterator tests");
for byte in b_from_c_ptr do
  writeln(byte, " as ", byte.type:string);
writeln();

// TEST BASICS
writeln("Basic functions");
var emptyBytes: bytes = "";
writeln("Is emptyBytes empty: ", emptyBytes.isEmpty());
writeln("Is emptyBytes space: ", emptyBytes.isSpace());
writeln();

var allWS = (" " + "\t" + "\n" + "\r" + "\v" + "\f"):bytes;
writeln("Is allWS space: ", allWS.isSpace());

// TEST SLICE
writeln("Slice tests");
writeln(b[..4], " -- the type is ", b[1..4].type:string); // "this"
writeln(b[6..], " -- the type is ", b[6..].type:string); // "is a bytes"
writeln(b[..], " -- the type is ", b[..].type:string); // "this is a bytes"
writeln(b[11..b.length], " -- the type is ", b[..].type:string); // "bytes"
writeln();

// TEST SEARCH ETC. -- this is getting too much without coercion
writeln("Search tests");
var bytes_to_search = new bytes("start lorem ipsum end");
writeln(bytes_to_search.startsWith("start":bytes));  //t
writeln(bytes_to_search.startsWith("lorem":bytes, "ipsum":bytes));  //f
writeln(bytes_to_search.startsWith("end":bytes)); //f
writeln(bytes_to_search.endsWith("start":bytes));  //f
writeln(bytes_to_search.endsWith("lorem":bytes, "ipsum":bytes));  //f
writeln(bytes_to_search.endsWith("end":bytes));   //t

writeln(b.find("is":bytes)); //3
writeln(b.rfind("is":bytes)); //6
writeln(b.count("is":bytes)); //2

writeln(b.find("is":bytes, region=4..)); //6
writeln(b.rfind("is":bytes, region=..5)); //2
writeln(b.count("is":bytes, region=..5)); //1

writeln(b.find("is is":bytes));
writeln("Make it plural ", b.replace("is is":bytes, "ese are":bytes));
writeln();

// TEST SPLIT/JOIN etc
writeln("Test split no args");
for (num,byte) in zip(1.., b.split()) do
  writeln("Split ", num, ": ", byte);

writeln("Test split with args");
for (num,byte) in zip(1.., b.split("is":bytes)) do
  writeln("Split ", num, ": ", byte);

// TEST CASTS
writeln("Type after string->byte ", ("test":bytes).type:string);


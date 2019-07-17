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

// TODO test localize

// c_str is tested with writeln anyways

// TEST ACCESSORS
writeln("Accessor tests");
for i in 1..b_from_c_ptr.length do
  writeln(b_from_c_ptr[i], " as ", b_from_c_ptr[i].type:string);
writeln();

// TEST SLICE

// TEST SEARCH ETC. -- this is getting too much without coercion
writeln("Search tests");
var bytes_to_search = new bytes("start lorem ipsum end");
writeln(bytes_to_search.startsWith("start":bytes));  //t
writeln(bytes_to_search.startsWith("lorem":bytes, "ipsum":bytes));  //f
writeln(bytes_to_search.startsWith("end":bytes)); //f
writeln(bytes_to_search.endsWith("start":bytes));  //f
writeln(bytes_to_search.endsWith("lorem":bytes, "ipsum":bytes));  //f
writeln(bytes_to_search.endsWith("end":bytes));   //t
writeln();

// TEST CASTS
writeln(("test":bytes).type:string);


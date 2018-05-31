config param my_start = 1;

enum blah { a=1, b=1, c };
writeln(blah.a < blah.b);

enum blah2 { a=my_start, b=my_start, c };
writeln(blah2.a < blah2.b);

enum blah3 { a=-1, b=-1, c };
writeln(blah3.a < blah3.b);

enum blah4 { a=-my_start, b=-my_start, c };
writeln(blah4.a < blah4.b);

enum blah5 { a, b=-1, c };
writeln(blah5.a < blah5.b);

enum blah6 { a, b=-my_start, c };
writeln(blah6.a < blah6.b);

enum blah7 { a=2*1, b=1+1, c };
writeln(blah7.a < blah7.b);

enum blah8 { a=2*my_start, b=my_start+my_start, c };
writeln(blah8.a < blah8.b);


var example_range = 5..15;
writeln('Our example range was set to: ', example_range);
var example_array: [example_range] real;
writeln('Our example array is now: ', example_array);
example_array = 5;
writeln('When set to 5: ', example_array);
example_array = 1..11;
writeln('When set to a range: ', example_array);
// retrieve the 5th index
writeln(example_array[5]);
// set index 5 to a new value
example_array[5] = 99999;
writeln(example_array);



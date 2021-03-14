use Itertools;

var test_case_1: [1..5] real = [2.3, -3.4, 8.91, 12.04, 2.8];

var test_case_2: string = 'This is a test sentence.';

var test_case_3: range(?) = -3..21 by 3;


//  Testing with an integer array

forall element in repeat(test_case_1, 3) do writeln(element);
writeln();


//  Testing with a string

forall element in repeat(test_case_2, 5) do writeln(element);
writeln();


//  Testing with a range

forall element in repeat(test_case_3, 4) do writeln(element);

use Itertools;

var test_case_1: [1..5] real = [2.3, -3.4, 8.91, 12.04, 2.8];

var test_case_2: string = 'This is a test sentence.';

var test_case_3: range(?) = -3..21 by 3;

var test_case_4_dictDomain: domain(string) = {'Second', 'Third', 'First'};
var test_case_4_dict: [test_case_4_dictDomain] int;
test_case_4_dict['Second'] = 2;
test_case_4_dict['Third'] = 3;
test_case_4_dict['First'] = 1;


//  Testing with an integer array

forall (idx, element) in zip(repeat('Example', 5), repeat(test_case_1, 5)) do
  writeln(idx, ': ', element);
writeln();


//  Testing with a string

forall (idx, element) in zip(repeat('Example', 5), repeat(test_case_2, 5)) do
  writeln(idx, ': ', element);
writeln();


//  Testing with a range

forall (idx, element) in zip(repeat('Example', 4), repeat(test_case_3, 4)) do
  writeln(idx, ': ', element);
writeln();


//  Testing with an associative array

forall (idx, element) in zip(repeat(test_case_4_dictDomain, 5),
                             repeat(test_case_4_dict, 5)) do
  writeln(idx, ' - ', element);

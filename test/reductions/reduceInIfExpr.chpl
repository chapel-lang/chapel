var A: [1..10] int = 42;
writeln(if (+ reduce [a in A] a) == 10 * 42 then 'Pass' else 'Fail');

const sum = + reduce [a in A] a;
writeln(if sum == 10 * 42 then 'Pass' else 'Fail');

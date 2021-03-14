// test that various non-iterators are correctly identified as not being ref
// iterators and that calling isRefIter on non-iterators doesn't cause any
// issues (compiler errors, or whatever)


writeln(isRefIter(42));
writeln(isRefIter(42.type));
writeln(isRefIter(int));

writeln(isRefIter(1..10));
writeln(isRefIter((1..10).type));
writeln(isRefIter(range));

writeln(isRefIter({1..10}));
writeln(isRefIter({1..10}.type));

writeln(isRefIter([1..10]));
writeln(isRefIter([1..10].type));

proc retArr(size) throws {
  if size <= 0 then
    throw new owned IllegalArgumentError('choice() size must be greater than 0');

  var samples: [1..size] int;
  return samples;
}

try {
  var c = retArr(size=0);
  writeln('Error: size=0 did not throw error');
} catch e {
  writeln(e.message());
}

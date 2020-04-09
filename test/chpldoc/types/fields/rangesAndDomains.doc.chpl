record R {
  var isRange: range;

  var boundedRange = 1..10;
  var lowBoundedRange = 1..;
  var highBoundedRange = ..10;
  var unboundedRange = ..;
}

record D {
  var is1DDomain: domain(1);
  var is2DDomain: domain(2);
  var is3DDomain: domain(3);

  var bounded1DDomain = {1..10};
  var bounded2DDomain = {1..10, 1..10};
  var bounded3DDomain = {1..10, 1..10, 1..10};
}

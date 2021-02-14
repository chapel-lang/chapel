record R {
  var isRange: range;

  var boundedRange = 1..10;
  var lowBoundedRange = 1..;
  var highBoundedRange = ..10;
  var unboundedRange = ..;

  var boundedRangeBy = 1..10 by 2;
  var lowBoundedRangeBy = 1.. by 2;
  var highBoundedRangeBy = ..10 by 2;
  var unboundedRangeBy = .. by 2;

  var boundedRangeByAlign = 1..10 by 2 align 1;
  var lowBoundedRangeByAlign = 1.. by 2 align 1;
  var highBoundedRangeByAlign = ..10 by 2 align 1;
  var unboundedRangeByAlign = .. by 2 align 1;
}

record D {
  var is1DDomain: domain(1);
  var is2DDomain: domain(2);
  var is3DDomain: domain(3);

  var bounded1DDomain = {1..10};
  var bounded2DDomain = {1..10, 1..10};
  var bounded3DDomain = {1..10, 1..10, 1..10};

  var bounded1DDomainBy = {1..10 by 2};
  var bounded1DDomainByAlign = {1..10 by 2 align 1};


}

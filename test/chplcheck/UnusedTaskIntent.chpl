module UnusedTaskIntent {
  var A: [1..10] int = 10;
  var B: int = 1;

  forall 1..10 with (var x: int) {}

  forall 1..10 with (var x = B) {}

  foreach 1..10
    with (ref A,
          in B) {}

  coforall 1..10 with (+ reduce A) {}

  begin with (ref A,
  + reduce B) {}

  begin with
  (ref A) {}

  cobegin with
  (const in A) { }

  [1..10 with(ref A)] { ; }

  [1..10 with (in A,
  const in B)] { ; }

}

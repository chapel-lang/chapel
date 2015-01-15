config const n = 1e6:uint;

var space = {1..n};

var x : uint = 2;
forall i in space with (ref x) do
  x *= i;

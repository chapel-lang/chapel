foreach (i, _) in zip(1..10, 1..10) { i; }
foreach (_, j) in zip(1..10, 1..10) { j; }
foreach (_, _) in zip(1..10, 1..10) {}
foreach (i, (_, _)) in zip(1..10, foreach pair in zip(1..10, 1..10) do pair) { i; }
foreach (_, (_, _)) in zip(1..10, foreach pair in zip(1..10, 1..10) do pair) { i; }
foreach ((_, _), _) in zip(foreach pair in zip(1..10, 1..10) do pair, 1..10) { i; }

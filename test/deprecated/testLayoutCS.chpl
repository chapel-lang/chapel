use LayoutCS;

const D = {1..10, 1..10},
      SD: sparse subdomain(D) dmapped new dmap(new CS()) = [(1, 1), (10, 10)];

writeln(SD);

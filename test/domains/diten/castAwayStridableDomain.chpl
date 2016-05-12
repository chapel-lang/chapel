var stridableDom = {1..20 by 2};
var stridableDomUnit = {1..20 by 1};
var unstridableDom = {1..10};

writeln("before cast/assign unstridableDom=", unstridableDom);        // {1..10}
writeln("before cast/assign stridableDom=", stridableDom);            // {1..20 by 2}

unstridableDom = stridableDom: domain(1, stridable=false);

writeln("after cast/assign unstridableDom=", unstridableDom);         // {1..20}
writeln("after cast/assign stridableDom=", stridableDom);             // {1..20 by 2}

unstridableDom = stridableDomUnit.safeCast(domain(1, stridable=false));

writeln("after safeCast/assign unstridableDom=", unstridableDom);     // {1..20}
writeln("after safeCast/assign stridableDomUnit=", stridableDomUnit); // {1..20}

// halt reached - non-stridable range assigned non-unit stride
unstridableDom = stridableDom.safeCast(domain(1, stridable=false));

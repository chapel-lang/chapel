var stridableDom = {1..20 by 2};
var stridableDomUnit = {1..20 by 1}: domain(1, strides=strideKind.any);
var unstridableDom = {1..10};

writeln("before cast/assign unstridableDom=", unstridableDom);        // {1..10}
writeln("before cast/assign stridableDom=", stridableDom);            // {1..20 by 2}

unstridableDom = stridableDomUnit: domain(1, strides=strideKind.one);

writeln("after cast/assign unstridableDom=", unstridableDom);         // {1..20}
writeln("after cast/assign stridableDom=", stridableDom);             // {1..20 by 2}

try {
  unstridableDom = stridableDom: domain(1, strides=strideKind.one);
} catch e {
  writeln("caught ", e);
}

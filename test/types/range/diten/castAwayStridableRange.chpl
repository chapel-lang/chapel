var stridableRange = 1..20 by 2;
var stridableRangeUnit = 1..20 by 1;
var unstridableRange = 1..10;

writeln("before cast/assign unstridableRange=", unstridableRange); // {1..10}
writeln("before cast/assign stridableRange=", stridableRange); // {1..20 by 2}

unstridableRange = stridableRange: range;

writeln("after cast/assign unstridableRange=", unstridableRange); // {1..20}
writeln("after cast/assign stridableRange=", stridableRange); // {1..20 by 2}

unstridableRange = stridableRangeUnit.safeCast(range);

writeln("after safeCast/assign unstridableRange=", unstridableRange); // {1..20}
writeln("after safeCast/assign stridableRangeUnit=", stridableRangeUnit); // {1..20}

// halt reached - non-stridable range assigned non-unit stride
unstridableRange = stridableRange.safeCast(range);

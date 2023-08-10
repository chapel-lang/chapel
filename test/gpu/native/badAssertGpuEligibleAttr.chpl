@assertGpuEligible for x in 1..10 {}
@assertGpuEligible while true {}
@assertGpuEligible do {} while true;
@assertGpuEligible coforall x in 1..10 {}

@assertGpuEligible proc f() {}

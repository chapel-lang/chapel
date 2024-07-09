@assertOnGpu for x in 1..10 {}
@assertOnGpu while true {}
@assertOnGpu do {} while true;
@assertOnGpu coforall x in 1..10 {}

@assertOnGpu proc f() {}

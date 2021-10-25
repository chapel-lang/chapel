record Forwarder {
  forwarding var x: Forwardee;
}
record Forwardee {
  proc get10() { return 10; }
}
var forwardees: [1..4] Forwardee;
writeln(forwardees.get10()); // prints 10 10 10 10
var forwarders: [1..4] Forwarder;
writeln(forwarders.get10()); // Unresolved call


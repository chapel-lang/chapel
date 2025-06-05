import IO;
use Set;

type compartment = set(string);

type rucksack = (compartment, compartment);

// Creates a set out of a string
proc setFromString(s: string): (set(string),) {
    var comp: set(string);
    for c in s {
        comp.add(c);
    }

    return (comp,);
}

var lines = ["hello", "world"];
var A = setFromString(lines);
writeln(A);

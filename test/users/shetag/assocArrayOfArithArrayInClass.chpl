const coeffDom : domain(1) = {0..1};
var locIndices : domain(2*int);
var locNodes   : [locIndices] [coeffDom] real;

locIndices += (1,2);
writeln(locNodes[(1,2)].domain);
locNodes[(1,2)] = (1,2);
writeln(locNodes[(1,2)]);

class C {
    const coeffDom : domain(1) = {0..1};
    var locIndices : domain(2*int);
    var locNodes   : [locIndices] [coeffDom] real;

    proc init() {
        this.complete();
        locIndices += (1,2);
        writeln(locNodes[(1,2)].domain);
        locNodes[(1,2)] = (1,2);
        writeln(locNodes[(1,2)]);
    }
}

var c = new owned C();

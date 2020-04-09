record Node {
    var lvl, idx : int;
    var loc = (lvl+idx)%numLocales;

    proc this() {
        return (lvl, idx);
    }
}

record Coeff {
    var dom  : domain(1);
    var data : [dom] real;
}

var nodes  : domain(Node);
var coeffs : [nodes] Coeff;

const node = new Node(1, 2);
nodes.add(node);
writeln(nodes);

const dom = {1..2};
const data : [dom] real = (1, 2);
coeffs[node] = new Coeff(dom, data);
writeln(coeffs[node]);

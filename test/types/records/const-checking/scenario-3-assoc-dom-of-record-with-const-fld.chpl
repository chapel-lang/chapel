// This is motivated by test/users/shetag/assocDomOfRecord.chpl

record Node {
  const lvl: int;
}

var nodes: domain(Node);

const node = new Node(1);
nodes.add(node);
writeln(nodes);

// now switch var<->const
const nodesConst: domain(Node);
var nodeVar = new Node(1000);
// ... and initialize with a literal
const nodesConstInitializedTyped: domain(Node) = { nodeVar };
const nodesConstInitializedUntyped = { nodeVar };
//compilerWarning("nodesConstInitializedUntyped.type = ", nodesConstInitializedUntyped.type:string);
writeln(nodesConstInitializedTyped);
writeln(nodesConstInitializedUntyped);

/////////// Arrays over the above domain. ///////////

record Coeff {
  const dom: int;
}

var coeffs1: [nodes] Coeff;
// This is legitimately an error: coeffs1[node] = new Coeff(11);
// TODO: allow a way to initialize coeffs1 other than from a literal,
// e.g. by reading from a channel, wholesale or element-by-element.
writeln(coeffs1[node]);
writeln(coeffs1);

// Meanwhile initialize from a literal.

var coeffs2 = [ new Node(2) => new Coeff(22) ];
//compilerWarning("coeffs2.type = ", coeffs2.type:string);
writeln(coeffs2[new Node(2)]);
writeln(coeffs2);

var coeffs3: [nodes] Coeff = [ new Node(1) => new Coeff(33) ];
writeln(coeffs3[new Node(1)]);
writeln(coeffs3);

/////////// Same arrays, using 'const' instead of 'var'. ///////////

const coeffc1: [nodes] Coeff;
// TODO: allow a way to initialize coeffc1 other than from a literal,
// e.g. by reading from a channel, wholesale or element-by-element.
writeln(coeffc1[node]);
writeln(coeffc1);

const coeffc2 = [ new Node(4) => new Coeff(44) ];
writeln(coeffc2[new Node(4)]);
writeln(coeffc2);

const coeffc3: [nodes] Coeff = [ new Node(1) => new Coeff(55) ];
writeln(coeffc3[new Node(1)]);
writeln(coeffc3);

config const W:uint(8) = 20;

record myNode {
    var D: domain(1);
    var myArray: [D][1..2] uint(8);

    proc init(w: uint(8)) {
        this.D = {0..w:int};
        this.complete();
        this.myArray[w][1] = 1;
    }
}

proc main() {
    var node = new myNode(W);
    writeln("Look I made a node.");
    // BLC: I commented this out because users shouldn't use sizeof():
    //
    //    writeln("It is this big: ", sizeof(myNode));
    writeln("The array is ", node.myArray.size, " by ", node.myArray[0].size);
    ref A = node.myArray[W:int][1..2].reindex(0..1);
    ref B = node.myArray[W:int];
    var C: [2..4] int;
    ref CC = C.reindex(0..2);
    CC[0] = 5;
    A[1] = 2;
    writeln(A);
    writeln(B);
    writeln(node.myArray);
    writeln(C);
}

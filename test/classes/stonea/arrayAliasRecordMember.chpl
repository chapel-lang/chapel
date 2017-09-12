record Panel2D {
    var matrix;
    const rowOffset : int;
    const colOffset : int;
    const rows : int;
    const cols : int;

    ref panel =
        matrix[rowOffset..rowOffset+rows-1, colOffset..colOffset+cols-1].reindex(1..rows, 1..cols);
};


var A : [1..10, 1..10] int;
var B = new Panel2D(A, 2, 2, 5, 5);
forall (i,j) in B.panel.domain do
    B.panel(i,j) = 42;
writeln(A);


record row_t {
    var data: [1..10] uint(8);
}

var table: [1..10] row_t;

var row = new row_t();
row.data(1) = 5;

var another_row = new row_t();
another_row.data(1) = 8;
table = forall j in 1..10 do
    if j<= 5 then row
    else another_row;

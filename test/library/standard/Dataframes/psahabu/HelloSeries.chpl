use Dataframes;

var I = ["Matthew" => 1, "Mark" => 2, "Luke" => 3, "John" => 4, "Acts" => 5];
var A = [1, 2, 3, 4, 5];

var nums: TypedSeries(int, int) = new TypedSeries(A);
var newTestament: TypedSeries(int, string) = new TypedSeries(A, I);
writeln(nums);
writeln(newTestament);

iter stoi(const in s: [] string): int {
   for scol in s do {
      yield scol:int;
   }
}
var choice = "3,5";
var fruit = ["Apples", "Bananas", "Oranges", "Peaches", "Pineapples", "Figs"];
var scols = choice.split(",");
for i in stoi(scols) do {
   writeln(fruit[i-1]);
}

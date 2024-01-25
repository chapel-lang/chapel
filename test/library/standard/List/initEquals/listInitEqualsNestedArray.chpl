use List;

var lst1: list(list(string)) = [["1", "2", "3"], ["a","b","c"]];
writeln(lst1);

var lst2: list(list(list(string))) = [
  [["1", "2", "3"], ["a","b","c"]],
  [["4", "5", "6"], ["d","e","f"]]
];
writeln(lst2);

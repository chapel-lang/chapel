use List;

var lst1: list(list(string)) = [["1", "2", "3"], ["a","b","c"]];
writeln(lst1.type: string);
writeln(lst1);

var lst2: list(list(list(string))) = [
  [["1", "2", "3"], ["a","b","c"]],
  [["4", "5", "6"], ["d","e","f"]]
];
writeln(lst2.type: string);
writeln(lst2);

var lst3 = new list([1, 3, 5]);
writeln(lst3.type: string);
writeln(lst3);

var lst4 = new list([[1,2,3], [4,5,6]]);
writeln(lst4.type: string);
writeln(lst4);

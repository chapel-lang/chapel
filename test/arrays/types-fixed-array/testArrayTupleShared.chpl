class C { var x = 0; }

proc test() {
  var arr = [(new shared C(0), new shared C(0)),
             (new shared C(1), new shared C(1)),
             (new shared C(2), new shared C(2))
            ];
  writeln(arr);
}
test();


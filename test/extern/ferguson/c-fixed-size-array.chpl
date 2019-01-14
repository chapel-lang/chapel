    var x = c_malloc(int, 5): c_array(int, 5);
    
    for i in 0..#5 do
      x[i] = i;
    
    writeln(x);
    
    var a: c_array(int, 5);
    
    a = x;
    
    writeln(a);


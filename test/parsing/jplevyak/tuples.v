//
// Tuples 
//

// Homogeneous Tuple Literals

tuple0 : ();		print(tuple0);  // empty tuple
tuple1 : (1);		print(tuple1);	// not a tuple!
tuple2 : (1,2);		print(tuple2);

// Heterogenious Tuple Literals

tuple3 : (1, '2', 3.0);	print(tuple3);
tuple4 : (tuple3, "4"); print(tuple4);

// Indexing Tuples

//anInt : tuple3(1);

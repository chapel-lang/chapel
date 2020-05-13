/* Temporary dummy method to make sure I can compile this test */
proc string.dedent(columns=0, ignoreFirst=true) { return this; }

var testStrings = [

  // [0] Common use-case
  """a
     b
     c""".dedent(),

  // [1] Newlines before/after
  """
  a
  b
  c
  """.dedent(),

  // [2] Note there is trailing whitespace in this example:
  """  
  a
    
  b
  c
  """.dedent(),

  // [3] Remove 4 columns of indentation (there are 5 columns before b and c)
  """a
     b
     c""".dedent(columns=4),

  // [4] removing (up to) 10 columns (ignoring first line though)
  """ a
        b
         c
          d
           e""".dedent(columns=10),


  // [5] Don't ignore the indentation level of the first line
  """a
     b
     c""".dedent(ignoreFirst=false),

  // [6] Mixing tabs and whitespace
  """a
    b
  		c
  	 d
   	e
  """.dedent(),

  // [7]
  """
      a
    b
  c
    d
      e
  """.dedent(),

  // [8]
  """ a
      b
      c""".dedent(),

  // [9]
  """ a
      b
      c""".dedent(ignoreFirst=false),

  // [10]
  """ a
      b
      c""".dedent(ignoreFirst=false, columns=2),

  // [11]
  """ a
      b
      c""".dedent(ignoreFirst=true, columns=2),

  // [12]
  """ a
        b
         c
          d
           e""".dedent(),

  // [13]
  """      a
          b
         c
        d
       e""".dedent(),

  // [14]
  """
   a
    b
 c
  """.dedent(),

  // [15] Empty lines
  """
  a

  b
  c
  """.dedent(),

  // [16] Note there is trailing white space in this string
  """
  a
 
  b
  c
  """.dedent(),

  // [17] Note there is trailing white space in this string
  """
  a
  
  b
  c
  """.dedent(),
  ];


// Test driver loop
for i in testStrings.indices {
  writeln('[%i]'.format(i));
  writeln(testStrings[i]);
  writeln('---');
}

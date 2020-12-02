/* Test string.dedent() */
config type dataType = string;

var testStrings = [

  // 0 Common use-case
  (
  """0a
     0b
     0c""":dataType).dedent(),

  // 1 Newlines before/after
  (
  """
  1a
  1b
  1c""":dataType).dedent(),

  // 2 Note there is trailing whitespace in this example:
  (
  """  
  2a
    
  2b
  2c""":dataType).dedent(),

  // 3 Remove 4 columns of indentation (there are 5 columns before b and c)
  (
  """3a
     3b
     3c""":dataType).dedent(columns=4),

  // 4 removing (up to) 10 columns (ignoring first line though)
  (
  """ 4a
        4b
         4c
          4d
           4e""":dataType).dedent(columns=10),


  // 5 Don't ignore the indentation level of the first line
  (
  """5a
     5b
     5c""":dataType).dedent(ignoreFirst=false),

  // 6 Mixing tabs and whitespace
  (
  """6a
    6b
  		6c
  	 6d
   	6e""":dataType).dedent(),

  // 7
  (
  """
      7a
    7b
  7c
    7d
      7e""":dataType).dedent(),

  // 8
  (
  """ 8a
      8b
      8c""":dataType).dedent(),

  // 9 - ignoreFirst test
  (
  """ 9a
      9b
      9c""":dataType).dedent(ignoreFirst=false),

  // 10 ignoreFirst + columns test
  (
  """ 10a
      10b
      10c""":dataType).dedent(ignoreFirst=false, columns=2),

  // 11
  (
  """ 11a
      11b
      11c""":dataType).dedent(ignoreFirst=true, columns=2),

  // 12
  (
  """ 12a
        12b
         12c
          12d
           12e""":dataType).dedent(),

  // 13
  (
  """      13a
          13b
         13c
        13d
       13e""":dataType).dedent(),

  // 14
  (
  """
   14a
    14b
 14c""":dataType).dedent(),

  // 15 Empty lines
  (
  """
  15a

  15b
  15c""":dataType).dedent(),

  // 16 Note there is trailing white space in this string
  (
  """
  16a
 
  16b
  16c""":dataType).dedent(),

  // 17 Note there is trailing white space in this string
  (
  """
  17a
  
  17b
  17c""":dataType).dedent(columns=2),


  // 18 Ignore tabs when columns>0
  (
  """
   18a
  	18b
  		18c""":dataType).dedent(ignoreFirst=false, columns=3),

  // 19 no-op
  (
  "19a 19b":dataType).dedent(ignoreFirst=false),

  // 20 ensure no whitespace on final newline
  (
  """
  20a
  20b
  20c
  """:dataType).dedent(),

  // 21 ensure no whitespace on final newline
  (
  """
     21a
     21b
     21c
  """:dataType).dedent(),
  // 22 ensure 1 space removed on final newline
  (
  """
 22a
 22b
 22c
  """:dataType).dedent(),
  // 23 mixed tabs and spaces on empty lines
  (
  """
  23a
  23b
 	
	 
   23c
  	23d""":dataType).dedent(),

  // 24 no-op with ignoreFirst=true
  (
  "24a 24b":dataType).dedent(ignoreFirst=true),
  ];

proc main() {
  // Test driver loop
  for i in testStrings.indices {
    writeln(testStrings[i]);
    writeln('---');
  }
}

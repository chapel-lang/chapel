/* Temporary dummy method to make sure I can compile this test */
use Regexp;

// Using (?m: .. ) as a work-around for multiLine not working (#15689)
const reWhitespaceOnly = compile('''(?m:^[ \t]+$)''');
// Using (?m: .. ) as a work-around for multiLine not working (#15689)
//const reLeadingWhitespace = compile('''(^[\s\t]*)(?:[\s\t\n])''', multiLine=true);
const reLeadingWhitespace = compile('''(?m:^[ \t]+)''');

proc string.dedent(columns=0, ignoreFirst=true) {
  //writeln('[DEBUG]: string = \n', this);
  var margin = '';
  var text = reWhitespaceOnly.sub('', this);
  var indents = reLeadingWhitespace.matches(text);

  // Find longest leading string of spaces and tabs common to all lines
  for indent in indents {
    var match = text[indent[0].offset..#indent[0].size];
    //writeln('[DEBUG]: match = "', match, '"');

    if margin == '' {
      // Initialize margin
      margin = match;
    } else if match.startsWith(margin) {
      // Current indent is deeper than margin, continue
      continue;
    } else if margin.startsWith(match) {
      // Current indent is shallower than margin, change margin
      margin = match;
    }
    else {
      // Find largest common whitespace between current line and previous margin
      for i in margin.indices {
        if margin[i] != match[i] {
          margin = margin[..<i];
          break;
        }
      }
    }
  }

  if margin != '' {
    text = text.replace(margin, '');
  }

  return text;
}


var testStrings = [

  // [0] Common use-case
  """[0]
     b
     c""".dedent(),

  // [1] Newlines before/after
  """
  [1]
  b
  c
  """.dedent(),

  // [2] Note there is trailing whitespace in this example:
  """  
  [2]
    
  b
  c
  """.dedent(),

  // [3] Remove 4 columns of indentation (there are 5 columns before b and c)
  """[3]
     b
     c""".dedent(columns=4),

  // [4] removing (up to) 10 columns (ignoring first line though)
  """ [4]
        b
         c
          d
           e""".dedent(columns=10),


  // [5] Don't ignore the indentation level of the first line
  """[5]
     b
     c""".dedent(ignoreFirst=false),

  // [6] Mixing tabs and whitespace
  """[6]
    b
  		c
  	 d
   	e
  """.dedent(),

  // [7]
  """
      [7]
    b
  c
    d
      e
  """.dedent(),

  // [8]
  """ [8]
      b
      c""".dedent(),

  // [9]
  """ [9]
      b
      c""".dedent(ignoreFirst=false),

  // [10]
  """ [10]
      b
      c""".dedent(ignoreFirst=false, columns=2),

  // [11]
  """ [11]
      b
      c""".dedent(ignoreFirst=true, columns=2),

  // [12]
  """ [12]
        b
         c
          d
           e""".dedent(),

  // [13]
  """      [13]
          b
         c
        d
       e""".dedent(),

  // [14]
  """
   [14]
    b
 c
  """.dedent(),

  // [15] Empty lines
  """
  [15]

  b
  c
  """.dedent(),

  // [16] Note there is trailing white space in this string
  """
  [16]
 
  b
  c
  """.dedent(),

  // [17] Note there is trailing white space in this string
  """
  [17]
  
  b
  c
  """.dedent(),
  ];


// Test driver loop
for i in testStrings.indices {
  //writeln('[%i]'.format(i));
  writeln(testStrings[i]);
  writeln('---');
}

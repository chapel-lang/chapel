use IO;

var s1 = """This is a string with multiple lines.
  It uses a triple quotation mark to open and close the string.
  Newline escapes are not interpreted: \n <--no newline
  Tab escapes are not interpreted: \t <--no tab
  Literal tabs are fine:	<--tab
  Backslashes don't do anything\ even at the end of lines \
  You can have two quotation marks next to eachother, but not three.  ""
  You can have as many single quotes as you like: ''''''''''''''''''
""";

var s2 = '''This is a string with multiple lines.
  It uses a triple single-quote to open and close the string.
  Newline escapes are not interpreted: \n <--no newline
  Tab escapes are not interpreted: \t <--no tab
  Literal tabs are fine:	<--tab
  Backslashes don't do anything\ even at the end of lines \
  You can have two single-quotes next to eachother, but not three.  ''
  You can have as many double-quotes as you like: """"""""""""""""""
''';

writeln(s1);
writeln(s2);

var s3 = """
# multiline strings are useful for
# inserting code blocks as strings

SELECT
  a_column
, another_column
FROM
  {%s}
WHERE
  {%s} = {%s};
""";

writeln(s3);

writeln("string.format() works on multiline strings");
writeln(s3.format("dinner", "table", "mahogany"));

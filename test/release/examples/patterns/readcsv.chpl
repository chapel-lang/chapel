/* This example demonstrates how to read in a CSV file.

   The example csv file is from Kaggle,
   https://www.kaggle.com/allen-institute-for-ai/CORD-19-research-challenge?select=metadata.csv
   The included metadata-20lines.csv only has 20 lines of data including the
   header row that has the column names.

   Each line in a csv file is a row of data.  Columns are separated by commas.
   Some of the cells of data have quotes because the data string itself 
   includes a comma or commas.

   There are many options in terms of data structures that such data could be
   read into.  Here are the two approaches illustrated here.
       (1) python-like -- list of Map instances where there is an entry in the
               Map per column (key=column name, value=column value for that row)
       (2) chapeltastic -- associative array with the key being the column name
               and the value being an array of strings

   To run this example, first make sure you have an installation of the
   Chapel compiler. For instructions on how to do that see
   https://chapel-lang.org/docs/master/usingchapel/building.html

   You can compile and run this example on the command line by doing a
       source util/quickstart/setchplenv.bash
   in the Chapel home directory, and then compiling and running this file 
   as follows:
       cd $CHPL_HOME/test/release/examples/patterns/
       chpl readcsv.chpl
       ./readcsv

  You can check that the output files for both approaches end up the same 
  as the input file by doing the following on the command line:
      diff metadata-20lines.csv readcsv-out1.csv
      diff metadata-20lines.csv readcsv-out2.csv
  If you do not get any output then the files are the same and don't have 
  any differences.

  References:
    file readers and writers
      https://chapel-lang.org/docs/main/modules/standard/IO.html#i-o-overview
      https://chapel-lang.org/docs/primers/fileIO.html

    lists
      https://chapel-lang.org/docs/primers/listOps.html
      https://chapel-lang.org/docs/master/modules/standard/List.html

    maps
      https://chapel-lang.org/docs/main/modules/standard/Map.html
 */
use IO;
use List;
use Map;

config const inFileName = "metadata-20lines.csv";
config const approach1FileName = "readcsv-out1.csv";
config const approach2FileName = "readcsv-out2.csv";
config const debug = true;

// Open up a file to work with.
var f = open(inFileName, ioMode.r);

// Now read the in the csv data. 
// Approach 1: formatted input into a list of maps
{
  writeln();
  writeln();
  writeln("Approach 1: formatted input into a list of maps");

  var reader = f.reader();

  // Read the first line to get the column names.
  // Assuming the following format, where the number of columns (ncol) is not 
  // known ahead of time:
  //    col_name_1,col_name_2,...,col_name_ncol\n
  var line : string;

  if (!reader.readLine(line)) then
    halt("ERROR: ", inFileName, " appears to be empty");

  var colNames = createListOfColNames(line);

  if debug {
    writeln("\ncolNames: ", colNames);
  }

  // Create a list of maps with one map per row of data.
  var dataRows : list(map(string, string));
  // Read individual lines from the file until the end of the file.
  while (reader.readLine(line)) {
    // Create a new map per line.
    var aRowMap = new map(string, string);
    var start = 0;      // index into the line string
    var colIdx = 0;     // the first column is indexed as zero
    // Process each line by finding the next field value from the 
    // start index to the next comma.
    for colName in colNames {
      var (nextVal, nextCommaIdx) = nextField(line, start);
      aRowMap.add(colName, nextVal);
      // Next field of data will start after the comma.
      start = nextCommaIdx + 1; 
    }
    dataRows.pushBack(aRowMap);
  }
  reader.close();

  // Print the last data row to illustrate what one of the row maps contains.
  if debug then
    writeln("\ndataRows[dataRows.size-1] = ", dataRows[dataRows.size-1]);
 
  //-----------------------------------------------------------------------
  // To see how the data is accessed and for testing, let's generate a 
  // temporary csv file from the stored data.  It should be the same as the 
  // input csv file.
  if debug {
    var outfile = open(approach1FileName, ioMode.cw);
    var writer = outfile.writer();

    // First write to the output file the column names separated by commas.
    for colIdx in 0..colNames.size-2 {
      writer.write(colNames[colIdx], ",");
    }
    // The last column name should not be followed by comma.
    writer.writeln(colNames[colNames.size-1]);

    // Then write to the file each row of data.
    for row in dataRows {
      for colIdx in 0..colNames.size-2 {
        writer.write(row[colNames[colIdx]], ",");
      }
      // The last column value should not be followed by comma.
      writer.writeln(row[colNames[colNames.size-1]]);
    }
    writer.close();
    outfile.close();
  }
}

// Approach 2: chapeltastic -- create an associative array with the key being 
//             the column name and the value being an array of strings
{
  writeln();
  writeln();
  write("Approach 2: associative array where keys are column names, ");
  writeln("and values are arrays of row values"); 

  // Create another reader of the input csv file
  var reader = f.reader();

  // Read the first line to get the column names.
  // Note: this portion is the same as in Approach 1
  var line : string;

  if (!reader.readLine(line)) then
    writeln("ERROR: ", inFileName, " appears to be empty");

  var colNames = createListOfColNames(line);

  if debug {
    writeln("\ncolNames: ", colNames);
  }

  // Create a list of lines from the file, so we know how many lines of 
  // data there are.  Then create an associative array with an entry per 
  // column of data, with the associative key being the column name.  
  // The value will be an array of strings with the column's value for 
  // each row in the data set.
  var dataRows : list(string);

  // Reading all of the lines of the file into a list.
  while (reader.readLine(line)) {
    dataRows.pushBack(line);
  }

  // Declaring an associative array, where the value type is a 1D array 
  // large enough to store one column value per row of data.
  var valDomain = {0..dataRows.size-1}; // Domain for values per column
  // An associative domain of strings
  var colNameDomain: domain(string) = colNames;

  // colData is an associative array indexed by the column name domain with 
  // array values.  Each array value will have number of rows string elements.
  var colData: [colNameDomain] [valDomain] string;

  // Processing each row to put the column values into the associative array.
  // See https://chapel-lang.org/docs/main/users-guide/base/zip.html for
  // information about zippered iteration.
  for (line, rowIdx) in zip(dataRows, valDomain) {
    // Process each line by finding the next field value from the start 
    // index to the next comma.
    // Doing work quite similar to what is done in Approach 1, but using
    // a Chapel iterator (see fieldsInRow() definition below) and storing
    // the field values in the associative array instead of a list of maps.
    for (colName, field) in zip(colNames, fieldsInRow(line)) {
      colData[colName][rowIdx] = field;
    }
  }
  reader.close();
  f.close();

  // Print out all of the data values for the first column of data.
  if debug then writeln("\ncolData[colNames[0]]=",colData[colNames[0]]);

  //-----------------------------------------------------------------------
  // To see how the data is accessed and for testing, let's generate a 
  // temporary csv file from the stored data.  It should be the same as the 
  // input csv file.
  if debug {
    var outfile = open(approach2FileName, ioMode.cw);
    var writer = outfile.writer();

    // First write to the output file the column names separated by commas.
    for colIdx in 0..colNames.size-2 {
      writer.write(colNames[colIdx],",");
    }
    // The last column name should not be followed by comma.
    writer.writeln(colNames[colNames.size-1]);  

    // Then write to the file each row of data.
    for rowIdx in 0..colData[colNames[0]].size-1 {
      for colIdx in 0..colNames.size-2 {
        writer.write(colData[colNames[colIdx]][rowIdx],",");
      }
      // The last column value should not be followed by comma.
      writer.writeln(colData[colNames[colNames.size-1]][rowIdx]);
    }
    writer.close();
    outfile.close();
  }
}

// fieldsInRow is an example of a user-defined iterator.  When called on
// a line from a csv file, it will yield the next column value each time
// it is called in a loop.
// See https://chapel-lang.org/docs/primers/iterators.html for some examples.
iter fieldsInRow(line) {
  var start = 0;
  while start<line.size {
    var (nextVal, nextCommaIdx) = nextField(line, start);
    start = nextCommaIdx + 1; // The next field will start after the comma.
    yield nextVal;
  }
}

// Returns the index of the next comma or the string length if no
// next comma is found.  Commas inside double quoted strings will be ignored.
proc findNextCommaNotInQuotes(str : string, start : int) {
  var inDoubleQuotes = false;
  for i in start..(str.size-1) {
    if !inDoubleQuotes && str[i]==',' then return i;
    if str[i]=='"' then inDoubleQuotes = !inDoubleQuotes;
  }
  return str.size;
}

// Given a line from a csv file and a starting index
// returns a tuple of the string starting at the starting index and
// ending right before the next comma not in a quoted string, and
// the index of the next comma.
//      input: "ab,cd,"ef",f"   and start=3
//      output: "cd"
//
//      input: "ab,cd,"ef",f"   and start=6
//      output: ""ef""
// Assumes that the starting index given is not within a quoted string.
proc nextField(line:string, start:int) {
    var commaIdx = findNextCommaNotInQuotes(line,start);
    var fieldVal = line[start..(commaIdx-1)];
    return (fieldVal.strip(),commaIdx);
}

// Given the string that is the first line of a csv file that has column names
// returns a list of column names in order.
proc createListOfColNames(line : string) {
  var colNames : list(string);
  var start = 0;
  while (start<line.size) {
    var (nextVal,commaIdx) = nextField(line,start);
    colNames.pushBack(nextVal);
    start = commaIdx+1;
  }
  return colNames;
} 

/* This example demonstrates how to read in a CSV file.

   The example csv file is from Kaggle,
   https://www.kaggle.com/allen-institute-for-ai/CORD-19-research-challenge?select=metadata.csv
   The included metadata-20lines.csv only has 20 lines of data including the
   header row that has the column names.

   Each line in csv files is a row of data.  Columns are separated by commas.
   Some of the cells of data have quotes because the data string itself includes
   a comma or commas.

   There are many options in terms of data structures that such data could be
   read into.  Here are the two approaches illustrated here.
       (1) pythonic -- list of Map instances where there is an entry in the Map
               per column (key=column name, value=column value for that row)
       (2) chapeltastic -- associative array with the key being the column name
               and the value being an array of strings

   You can compile and run this example by doing a
       source util/quickstart/setchplenv.bash
   in the chapel home directory, and then compiling and running this file as follows:
       chpl --fast readcsv.chpl
       ./readcsv
 */
use IO;
use List; // FIXME: put in a link to the list primer
use Map;

config const fileName = "metadata-20lines.csv";
config const debug = true;

// Open up a file to work with.
var f = open(fileName, iomode.r);

// Now read the in the csv data. Approach 1: formatted input into a list of maps
{
  var reader = f.reader();

  // read the first line to get the column names
  // assuming the following format, where the number of columns (ncol) is not 
  // known ahead of time:
  //    col_name_1,col_name_2,...,col_name_ncol\n
  var line : string;

  if (!reader.readline(line)) then
    writeln("ERROR: ", fileName, " appears to be empty");

  var colNames : list(string);
  colNames = createListOfColNames(line);

  if debug {
    writeln("line = ", line);
    writeln("colNames: ", colNames);
  }

  // Create a list of maps with one map per row of data.
  var dataRows : list(map(string,string));

  while (reader.readline(line)) {
    var aRowMap = new map(string, string);
    var start = 0;
    var colIdx = 0;
    while (start<line.size && colIdx<colNames.size) {
      var (nextVal,nextCommaIdx) = nextField(line,start);
      aRowMap.add(colNames[colIdx],nextVal);
      start = nextCommaIdx + 1;
      colIdx = colIdx + 1;
    }
    dataRows.append(aRowMap);
    if debug then writeln("aRowMap = ", aRowMap);
  }

  if debug then writeln("dataRows = ", dataRows);

  reader.close();
}

{
  // Approach 2: chapeltastic -- create an associative array with the key being the 
  //             column name and the value being an array of strings
  var reader = f.reader();

  // read the first line to get the column names
  // assuming the following format, where the number of columns (ncol) is not 
  // known ahead of time:
  //    col_name_1,col_name_2,...,col_name_ncol\n
  // Note: this portion is the same as in approach (1)
  var line : string;

  if (!reader.readline(line)) then
    writeln("ERROR: ", fileName, " appears to be empty");

  var colNames : list(string);
  colNames = createListOfColNames(line);

  if debug {
    writeln("line = ", line);
    writeln("colNames: ", colNames);
  }

  // Create a list of lines from the file, so we know how many lines of data there are.
  // Then create an associative array with an entry per column of data, with the
  // associative key being the column name.  The value will be an array of strings
  // with the column's value for each row in the data set.
  // FIXME: should show an example in these comments
  var dataRows : list(string);

  // Reading all of the lines of the file into a list.
  while (reader.readline(line)) {
    dataRows.append(line);
  }

  // Declaring an associative array, where the value type is an array large enough
  // to store one column value per row of data.
  var valDomain = {0..dataRows.size};
  var colNameDomain: domain(string);
  for colName in colNames do
    colNameDomain += colName;
  if debug then writeln("colNameDomain = ", colNameDomain);
  var colData: [colNameDomain] [valDomain] string;

  // Processing each row to put the column values into the associative array.
  for rowIdx in 0..dataRows.size-1 {
    line = dataRows[rowIdx];
    var start = 0;
    var colIdx = 0;
    while (start<line.size && colIdx<colNames.size) {
      var (nextVal,nextCommaIdx) = nextField(line,start);
      colData[colNames[colIdx]][rowIdx] = nextVal;
      start = nextCommaIdx + 1;
      colIdx = colIdx + 1;
    }
  }
  reader.close();

  // For testing, let's generate a temporary csv file from the stored data.  It should
  // be the same as the input csv file.
  // Reference: https://chapel-lang.org/docs/main/modules/standard/IO.html#i-o-overview
  if debug {
    var outfile = open("readcsv-outfile.csv", iomode.cw);
    var writer = outfile.writer();

    // First write to the output file the column names separated by commas
    for colIdx in 0..colNames.size-2 {
      writer.write(colNames[colIdx],",");
    }
    writer.write(colNames[colNames.size-1]);  // last column should not be followed by comma

    // Then write to the file each row of data.
    for rowIdx in 0..colData[colNames[0]].size-1 {
      for colIdx in 0..colNames.size-2 do
        writer.write(colData[colNames[colIdx]][rowIdx],",");
      writer.write(colData[colNames[colNames.size-1]][rowIdx]);
    }
  }

}

// Returns the index of the next comma or the string length if no
// next comma is found.  Commas inside quoted strings will be ignored.
proc findNextCommaNotInQuotes(str : string, start : int) {
  var inQuotes = false;
  for i in start..(str.size-1) {
    if !inQuotes && str[i]==',' then return i;
    if str[i]=='"' then inQuotes = !inQuotes;
  }
  return str.size;
}

// Given a line from a csv file and a starting index
// returns a tuple of the string starting at the starting index and
// ending right before the next comma not in a quoted string, and
// the index of the next comma.
//      input: "ab,cd,"ef",f"   and start=3
//      output: "cd"
//      input: "ab,cd,"ef",f"   and start=6
//      output: ""ef""
// Assumes that the starting index given is not within a quoted string.
proc nextField(line:string, start:int) {
    var commaIdx = findNextCommaNotInQuotes(line,start);
    return (line[start..(commaIdx-1)],commaIdx);
}

// Given the string that is the first line of a csv file that has column names
// returns a list of column names in order.
proc createListOfColNames(line : string) {
  var colNames : list(string);
  var start = 0;
  while (start<line.size) {
    var (nextVal,commaIdx) = nextField(line,start);
    colNames.append(nextVal);
    start = commaIdx+1;
  }
  return colNames;
} 

use BlockDist;

/* Wrapper concrete class for generic class. */
class GenProperty {
  var dataType: int;
}

/* Wrapped generic class to hold arrays of variable size and type. */
class Property: GenProperty {
  type etype;
  var propertyIdentifier: domain(int);
  var propertyValue: [propertyIdentifier] etype;
}

/** Given input data from dataArray and the vertices (rows) to insert the data,
* insert every key:value pair to the corresponding element of vertexProps. */
proc populate(dataArray, inputVertices, ref vertexProps, dataTypeMap, colId) {
  var etype = dataArray.eltType:string;
  var etypeInd:int = dataTypeMap.find(etype);
  writeln("## Inserting ", etype, " data ##");
  forall (v,j) in zip(inputVertices,inputVertices.domain) {
    writeln("Attempting to insert data ", dataArray[j], " for vertex ", inputVertices[j]);
    ref currentProperty: shared Property(?) = vertexProps[v,etypeInd];
    currentProperty!.dataType = etypeInd;
    //currentProperty!.propertyIdentifier += colId; // error
    //currentProperty!.propertyValue[colId] = dataArray[j]; // error
  }
  writeln();
}

/** Assume we have columnar data formatted as below, the goal is to store the
* data for every vertex compactly to allow for easy retrieval.
*
* v    id   subscribed   birthyear
* 0    27         true        1994
* 1    21        false        1996
* 2    19         true        1945
* 5    28        false        1976
* 7    45         true        2001
* 9    21         true        1998 */

/** Using Arkouda, all data is preprocessed, vertex names are mapped to the
* range [0,n-1] where n is the number of vertices. We will end up with with the
* arrays below. */
var inputVertices = [0, 1, 2, 5, 7, 9];                      // colId 0
var id = [27, 21, 19, 28, 45, 21];                           // colId 1
var subscribed = [true, false, true, false, true, true];     // colId 2
var birthyear = [1994, 1996, 1945, 1976, 2001, 1998];        // colId 3

const numVertices = 10; // Does not need to directly map with the values in inputVertices.
const numDataTypes = 2; // Will be the same as the size of dataTypeMap, in real scenarios this
                        // varies with the input.
var dataTypeMap = ["int(64)", "bool"];

/** Create block distributed two-dimensional array where the row indices
* correspond to the vertex being stored and the column indices to the datatype
* being stored. Each element of the array is to store an object of class
* Property that contains an associative array where the domain is an integer
* identifier for the name of the property (column) being stored and the element
* is the value for that vertex in that column. */
var vertexProps = blockDist.createArray({0..<numVertices, 0..<numDataTypes},
                                        shared GenProperty?);

/** Initialize each array element in the two-dimensional vertexProps array. Not
* sure if this is necessary, or if it can be done on-the-fly within populate if
* vertexProps[v,etypeInd] is nil, but that might cause a race condition if two
* threads both encounter a nil and try to create their own Property objects at
* that array location. */
forall (v,d) in vertexProps.domain {
  var datatype = dataTypeMap[d];
  if datatype == "int(64)" {
    var newDom: domain(int);
    var newArr: [newDom] int;
    vertexProps[v,d] = new shared Property(d, int, newDom, newArr);
  }
  if datatype == "bool" {
    var newDom: domain(int);
    var newArr: [newDom] bool;
    vertexProps[v,d] = new shared Property(d, bool, newDom, newArr);
  }
}

/** Below we populate the vertexProps array using the populate procedure defined
above. In the actual code, we loop over an array of strings that identifier the
array to extract from the Arkouda symbol table, but for simplicity sake, we just
call the method 3 times for each array. */
populate(id, inputVertices, vertexProps, dataTypeMap, 1);
populate(subscribed, inputVertices, vertexProps, dataTypeMap, 2);
populate(birthyear, inputVertices, vertexProps, dataTypeMap, 3);

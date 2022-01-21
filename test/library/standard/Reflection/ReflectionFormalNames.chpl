use Reflection;

record r { var x = 0; param y = 0; type z = int; }

param idxValue = 0;
param idxParam = 1;
param idxType = 2;
param nameValue = "x";
param nameParam = "y";
param nameType = "z";

// proc getFieldName(param int) param: string;
getFieldName(r, idxValue);          // OK
getFieldName(r, idx=idxValue);      // OK
getFieldName(t=r, idxValue);        // OK
getFieldName(t=r, idx=idxValue);    // OK

var obj = new r();

// proc getField(any, param int) param;
getField(obj, idxParam);            // OK
getField(obj, idx=idxParam);        // OK
getField(obj=obj, idxParam);        // OK
getField(obj=obj, idx=idxParam);    // OK

// proc getField(any, param int) type;
getField(obj, idxType);             // OK
getField(obj, idx=idxType);         // OK
getField(obj=obj, idxType);         // OK
getField(obj=obj, idx=idxType);     // OK

// proc getField(any, param int) const ref;
getField(obj, idxValue);            // OK
getField(obj, idx=idxValue);        // OK
getField(obj=obj, idxValue);        // OK
getField(obj=obj, idx=idxValue);    // OK

// proc getField(any, param string) param;
getField(obj, nameParam);             // OK
getField(obj, name=nameParam);        // OK
getField(obj=obj, nameParam);         // OK
getField(obj=obj, name=nameParam);    // OK

// proc getField(any, param string) type;
getField(obj, nameType);              // OK
getField(obj, name=nameType);         // OK
getField(obj=obj, nameType);          // OK
getField(obj=obj, name=nameType);     // OK

// proc getField(any, param string) const ref;
getField(obj, nameValue);             // OK
getField(obj, name=nameValue);        // OK
getField(obj=obj, nameValue);         // OK
getField(obj=obj, name=nameValue);    // OK

// proc getFieldIndex(type, param string) param: int;
getFieldIndex(r, nameParam);          // OK
getFieldIndex(t=r, nameParam);        // OK
getFieldIndex(r, name=nameParam);     // OK
getFieldIndex(t=r, name=nameParam);   // OK

// proc hasField(type, param string) param: bool;
hasField(r, nameParam);           // OK
hasField(t=r, nameParam);         // OK
hasField(r, name=nameParam);      // OK
hasField(t=r, name=nameParam);    // OK

// proc isFieldBound(type, param int) param: bool;
isFieldBound(r, idxParam);          // OK
isFieldBound(t=r, idxParam);        // OK
isFieldBound(r, idx=idxParam);      // OK
isFieldBound(t=r, idx=idxParam);    // OK

// proc isFieldBound(type, param string) param: bool;
isFieldBound(r, nameParam);           // OK
isFieldBound(t=r, nameParam);         // OK
isFieldBound(r, name=nameParam);      // OK
isFieldBound(t=r, name=nameParam);    // OK


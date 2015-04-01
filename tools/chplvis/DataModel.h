#ifndef DATAMODEL_H
#define DATAMODEL_H

class DataModel {

  int LoadFile (const char *filename);

 public:

  // 
  DataModel() { numLocales = -1; }

  //  LoadFile loads data from a collection of files
  //  filename of the form  basename-n, where n can
  //  be a multi-digit number
  //  Returns 1 if successful, 0 if not

  int LoadData (const char *filename);

  //  Number of locales found in loading files

  int NumLocales () { return numLocales; }

 private:

  int numLocales;

};


#endif

/* printing.h 
 * This file contains convenience functions which can print stuff for me
 * so I don't have to write them too many times
 */
#ifndef PRINTING_H
#define PRINTING_H
#include <stdlib.h>
#include <arrow-glib/arrow-glib.h>

void print_array(GArrowArray *array);

void print_record_batch(GArrowRecordBatch *record_batch);
#endif /* PRINTING_H */

//
// THESE FIRST TWO TESTS 0 AND 1 ARE COPIED FROM ../domains/doc-examples
//

iter someIndexIterator() do
  for i in 1..32 do
    yield i;

const parentDom = { 0..128 };

{
/* START_EXAMPLE_0 */
var spsDom: sparse subdomain(parentDom);
for i in someIndexIterator() do
  spsDom += i;
/* STOP_EXAMPLE_0 */

for i in someIndexIterator() do assert(spsDom.contains(i));
}

const N = 32;

{
/* START_EXAMPLE_1 */
var spsDom: sparse subdomain(parentDom);
var idxBuf = spsDom.createIndexBuffer(size=N);
for i in someIndexIterator() do
  idxBuf.add(i);
idxBuf.commit();
/* STOP_EXAMPLE_1 */

for i in someIndexIterator() do assert(spsDom.contains(i));
}

{
/* START_EXAMPLE_2 */
var dataSorted=true;
var spsDom: sparse subdomain(parentDom);
var idxBuf = spsDom.createIndexBuffer(size=N, dataSorted=dataSorted);
for i in someIndexIterator() do
  idxBuf.add(i);
idxBuf.commit();
/* STOP_EXAMPLE_2 */

for i in someIndexIterator() do assert(spsDom.contains(i));
}

{
/* START_EXAMPLE_3 */
var isUnique=true;
var spsDom: sparse subdomain(parentDom);
var idxBuf = spsDom.createIndexBuffer(size=N, isUnique=isUnique);
for i in someIndexIterator() do
  idxBuf.add(i);
idxBuf.commit();
/* STOP_EXAMPLE_3 */

for i in someIndexIterator() do assert(spsDom.contains(i));
}

{
/* START_EXAMPLE_4 */
var dataSorted=true;
var isUnique=true;
var spsDom: sparse subdomain(parentDom);
var idxBuf = spsDom.createIndexBuffer(size=N,
                                      dataSorted=dataSorted,
                                      isUnique=isUnique);
for i in someIndexIterator() do
  idxBuf.add(i);
idxBuf.commit();
/* STOP_EXAMPLE_4 */

for i in someIndexIterator() do assert(spsDom.contains(i));
}



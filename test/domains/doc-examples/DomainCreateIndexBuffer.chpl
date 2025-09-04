//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
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

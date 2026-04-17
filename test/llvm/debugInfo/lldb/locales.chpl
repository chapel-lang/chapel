proc main() {
  for l in Locales {
    writeln("Locale ", l.id);
  }
}




// CHECK: p Locales
// CHECK-LOCAL-NEXT: ([domain(1,int(64),one)] locale) [0..0] ChapelLocale::locale {
// CHECK-LOCAL-NEXT: [0] = {
// CHECK-LOCAL-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-LOCAL-NEXT: }
// CHECK-NOLOCAL-NEXT: (wide(ref(wide([domain(1,int(64),one)] locale)))) {

// CHECK: target var Locales
// CHECK-LOCAL-NEXT: (_ref(_array(unmanaged [domain(1,int(64),one)] locale))) Locales = 0x{{0*[0-9a-fA-F]+}} [0..0] ChapelLocale::locale: {
// CHECK-LOCAL-NEXT: [0] = {
// CHECK-LOCAL-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-LOCAL-NEXT: }
// CHECK-NOLOCAL-NEXT: (wide(ref(wide([domain(1,int(64),one)] locale)))) Locales = {

// CHECK-LOCAL: target var Locales[0]
// CHECK-LOCAL-NEXT: (ChapelLocale::locale) Locales[0] = {
// CHECK-LOCAL-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-LOCAL-NEXT: }

// CHECK-LOCAL: target var *Locales[0]._instance
// CHECK-LOCAL-NEXT: (ChapelLocale::BaseLocale) *Locales[0]._instance = {
// CHECK-LOCAL-NEXT: super = (cid = {{[0-9]+}})
// CHECK-LOCAL-NEXT: parent = {
// CHECK-LOCAL-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-LOCAL-NEXT: }
// CHECK-LOCAL-NEXT: nPUsLogAcc = {{[0-9]+}}
// CHECK-LOCAL-NEXT: nPUsLogAll = {{[0-9]+}}
// CHECK-LOCAL-NEXT: nPUsPhysAcc = {{[0-9]+}}
// CHECK-LOCAL-NEXT: nPUsPhysAll = {{[0-9]+}}
// CHECK-LOCAL-NEXT: maxTaskPar = {{[0-9]+}}
// CHECK-LOCAL-NEXT: numColocales = {{[0-9]+}}
// CHECK-LOCAL-NEXT: runningTaskCounter = {{[0-9]+}}
// CHECK-LOCAL-NEXT: }

// CHECK: p l
// CHECK-LOCAL-NEXT: (_ref(locale)) 0x{{0*[0-9a-fA-F]+}}: {
// CHECK-LOCAL-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-LOCAL-NEXT: }
// CHECK-NOLOCAL-NEXT: (wide(_ref(locale))) {

// CHECK-LOCAL: p *l._instance
// CHECK-LOCAL-NEXT: (ChapelLocale::BaseLocale) {
// CHECK-LOCAL-NEXT: super = (cid = {{[0-9]+}})
// CHECK-LOCAL-NEXT: parent = {
// CHECK-LOCAL-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-LOCAL-NEXT: }
// CHECK-LOCAL-NEXT: nPUsLogAcc = {{[0-9]+}}
// CHECK-LOCAL-NEXT: nPUsLogAll = {{[0-9]+}}
// CHECK-LOCAL-NEXT: nPUsPhysAcc = {{[0-9]+}}
// CHECK-LOCAL-NEXT: nPUsPhysAll = {{[0-9]+}}
// CHECK-LOCAL-NEXT: maxTaskPar = {{[0-9]+}}
// CHECK-LOCAL-NEXT: numColocales = {{[0-9]+}}
// CHECK-LOCAL-NEXT: runningTaskCounter = {{[0-9]+}}
// CHECK-LOCAL-NEXT: }

// CHECK-LOCAL: p l.id()
// CHECK-LOCAL-NEXT: (int(64)) 0

// CHECK-LOCAL: p l.maxTaskPar()
// CHECK-NOLOCAL: p l.addr.maxTaskPar()
// CHECK-NEXT: (int(64)) {{[0-9]+}}

// CHECK: p here().maxTaskPar()
// CHECK-NEXT: (int(64)) {{[0-9]+}}

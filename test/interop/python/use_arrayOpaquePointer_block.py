import arrayOpaquePointer_block

arrayOpaquePointer_block.chpl_setup()
x = arrayOpaquePointer_block.makeBlockArray()
arrayOpaquePointer_block.printBlock(x)

arrayOpaquePointer_block.addEltBlock(x, 2, 3)
arrayOpaquePointer_block.printBlock(x)

# clean up pointer
x.cleanup()

arrayOpaquePointer_block.chpl_cleanup()

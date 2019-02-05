import arrayOpaquePointer_block_with

arrayOpaquePointer_block_with.chpl_setup()
# use a with clause instead of explicitly cleaning it up
with arrayOpaquePointer_block_with.makeBlockArray() as x:
    arrayOpaquePointer_block_with.printBlock(x)

    arrayOpaquePointer_block_with.addEltBlock(x, 2, 3)
    arrayOpaquePointer_block_with.printBlock(x)

arrayOpaquePointer_block_with.chpl_cleanup()

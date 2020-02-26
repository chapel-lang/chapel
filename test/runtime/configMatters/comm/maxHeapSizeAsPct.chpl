use SysCTypes;

extern proc chpl_sys_physicalMemoryBytes(): uint(64);
const physMemSize_1pct: real = chpl_sys_physicalMemoryBytes():real * 0.01;

extern proc chpl_comm_getenvMaxHeapSize(): size_t;
const maxHeapSize: real = chpl_comm_getenvMaxHeapSize():real;

writeln(if abs(maxHeapSize - physMemSize_1pct) / physMemSize_1pct < 0.001
        then 'pass'
        else 'fail');

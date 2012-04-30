{
   zlib doesn't cleanup
   Memcheck:Cond
   fun:inflateReset2
   fun:inflateInit2_
}
{
   hwloc cannot free the global parser (xmlCleanupParser) because other threads may be using it
   Memcheck:Leak
   ...
   fun:xmlInitParser
}
{
   hwloc cannot free the global xml dict RMutex because it cannot call xmlCleanupParser
   Memcheck:Leak
   fun:malloc
   fun:xmlNewRMutex
   ...
   fun:xmlDictCreate
}

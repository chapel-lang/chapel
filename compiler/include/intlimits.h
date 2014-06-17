/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#include <limits>

#ifndef INT8_MIN
#define INT8_MIN (std::numeric_limits<int8_t>::min())
#endif
#ifndef INT16_MIN
#define INT16_MIN (std::numeric_limits<int16_t>::min())
#endif
#ifndef INT32_MIN
#define INT32_MIN (std::numeric_limits<int32_t>::min())
#endif
#ifndef INT64_MIN
#define INT64_MIN (std::numeric_limits<int64_t>::min())
#endif

#ifndef INT8_MAX
#define INT8_MAX (std::numeric_limits<int8_t>::max())
#endif
#ifndef INT16_MAX
#define INT16_MAX (std::numeric_limits<int16_t>::max())
#endif
#ifndef INT32_MAX
#define INT32_MAX (std::numeric_limits<int32_t>::max())
#endif
#ifndef INT64_MAX
#define INT64_MAX (std::numeric_limits<int64_t>::max())
#endif

#ifndef UINT8_MAX
#define UINT8_MAX (std::numeric_limits<uint8_t>::max())
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (std::numeric_limits<uint16_t>::max())
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (std::numeric_limits<uint32_t>::max())
#endif
#ifndef UINT64_MAX
#define UINT64_MAX (std::numeric_limits<uint64_t>::max())
#endif



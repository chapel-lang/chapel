#include <limits>

#ifndef INT8_MIN
#define INT8_MIN (std::numeric_limits<int8_t>::min())
#define INT16_MIN (std::numeric_limits<int16_t>::min())
#define INT32_MIN (std::numeric_limits<int32_t>::min())
#define INT64_MIN (std::numeric_limits<int64_t>::min())

#define INT8_MAX (std::numeric_limits<int8_t>::max())
#define INT16_MAX (std::numeric_limits<int16_t>::max())
#define INT32_MAX (std::numeric_limits<int32_t>::max())
#define INT64_MAX (std::numeric_limits<int64_t>::max())

#define UINT8_MAX (std::numeric_limits<uint8_t>::max())
#define UINT16_MAX (std::numeric_limits<uint16_t>::max())
#define UINT32_MAX (std::numeric_limits<uint32_t>::max())
#define UINT64_MAX (std::numeric_limits<uint64_t>::max())
#endif



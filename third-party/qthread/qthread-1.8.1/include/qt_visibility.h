#ifndef QT_VISIBILITY_H
#define QT_VISIBILITY_H

#if defined(_WIN32) || defined(__CYGWIN__)
# warning Building on windows has never been tested
# ifdef BUILDING_DLL
#  ifdef __GNUC__
#   define API_FUNC __attribute__((dllexport))
#  else
#   define API_FUNC __declspec(dllexport)       // Note: actually gcc seems to also supports this syntax.
#  endif
# else
#  ifdef __GNUC__
#   define API_FUNC __attribute__((dllimport))
#  else
#   define API_FUNC __declspec(dllimport)       // Note: actually gcc seems to also supports this syntax.
#  endif
# endif /* ifdef BUILDING_DLL */
# define INTERNAL
#else /* if defined(_WIN32) || defined(__CYGWIN__) */
# if __GNUC__ >= 4
#  define API_FUNC __attribute__((visibility("default")))
#  define INTERNAL __attribute__((visibility("hidden")))
# else
#  define API_FUNC
#  define INTERNAL
# endif
#endif /* if defined(_WIN32) || defined(__CYGWIN__) */

#endif /* ifndef QT_VISIBILITY_H */
/* vim:set expandtab: */

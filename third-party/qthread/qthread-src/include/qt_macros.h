#ifndef QT_MACROS_H
#define QT_MACROS_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_UNUSED
# define QUNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
# define QUNUSED(x) UNUSED_ ## x
#endif

#ifdef TLS
# define TLS_DECL(type, name)      TLS type name
# define TLS_DECL_INIT(type, name) TLS type name = 0
# define TLS_GET(name)             name
# define TLS_SET(name, val)        name = (val)
# define TLS_INIT(name)
# define TLS_INIT2(name, func)
# define TLS_DELETE(name)          name = 0
#else
# define TLS_DECL(type, name)      pthread_key_t name
# define TLS_DECL_INIT(type, name) pthread_key_t name
# define TLS_GET(name)             pthread_getspecific(name)
# define TLS_SET(name, val)        qassert(pthread_setspecific((name), (void *)(val)), 0)
# define TLS_INIT(name)            qassert(pthread_key_create(&(name), NULL), 0)
# define TLS_INIT2(name, func)     qassert(pthread_key_create(&(name), (func)), 0)
# define TLS_DELETE(name)          qassert(pthread_key_delete(name), 0)
#endif // ifdef TLS

#endif // ifndef QT_MACROS_H
/* vim:set expandtab: */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h> /* for getenv() and strtoul() */
#include <stdio.h>  /* for fprintf() */
#include <string.h>
#include <strings.h> /* for strcasecmp() */

#include "qt_visibility.h"
#include "qt_envariables.h"
#include "qt_debug.h"
#include "qt_asserts.h"

static int info = -1;

static const char *qt_internal_envstr(const char *envariable,
                                      const char *dflt)
{
    char        mod_envariable[100];
    const char *str;

    snprintf(mod_envariable, 100, "QT_%s", envariable);
    str = getenv(mod_envariable);
    qthread_debug(CORE_BEHAVIOR, "checking envariable %s\n", envariable);
    if (str && *str) {
        if (info) {
            printf("%s = %s\n", mod_envariable, str);
        }
        return str;
    } else {
        snprintf(mod_envariable, 100, "QTHREAD_%s", envariable);
        str = getenv(mod_envariable);
        if (str && *str) {
            if (info) {
                printf("%s = %s\n", mod_envariable, str);
            }
            return str;
        }
    }
    if (info) {
        if (dflt != NULL) {
            printf("[QT|QTHREAD]_%s = (unset) -> %s\n", envariable, dflt);
        } else {
            printf("[QT|QTHREAD]_%s = (unset)\n", envariable);
        }
    }
    return NULL;
}

static inline void check_info(void)
{
    if (-1 == info) {
        const char *str;
        str = getenv("QT_INFO");
        if (str == NULL) {
            str = getenv("QTHREAD_INFO");
        }
        if (str != NULL) {
            int   tmp;
            char *errptr;
            tmp = strtol(str, &errptr, 0);
            if (*errptr != 0) {
                info = 0;
            } else {
                info = (tmp > 1) ? 1 : 0;
            }
        } else {
            info = 0;
        }
    }
}

const char INTERNAL *qt_internal_get_env_str(const char *envariable,
                                             const char *dflt)
{
    assert(strlen(envariable) < 90);
    check_info();
    return qt_internal_envstr(envariable, dflt);
}

unsigned long INTERNAL qt_internal_get_env_num(const char   *envariable,
                                               unsigned long dflt,
                                               unsigned long zerodflt)
{
    const char   *str;
    unsigned long tmp = dflt;

    if (dflt != 0) {
        char dflt_str[10];

        snprintf(dflt_str, 10, "%lu", dflt);
        str = qt_internal_get_env_str(envariable, dflt_str);
    } else {
        str = qt_internal_get_env_str(envariable, NULL);
    }
    if (str && *str) {
        char *errptr;
        tmp = strtoul(str, &errptr, 0);
        if (*errptr != 0) {
            fprintf(stderr, "unparsable %s (%s)\n", envariable, str);
            tmp = dflt;
        }
        if (tmp == 0) {
            qthread_debug(CORE_DETAILS, "since envariable %s is 0, choosing default: %u\n", envariable, zerodflt);
            tmp = zerodflt;
        } else {
            qthread_debug(CORE_DETAILS, "envariable %s parsed as %u\n", envariable, tmp);
        }
    }
    return tmp;
}

unsigned char INTERNAL qt_internal_get_env_bool(const char   *envariable,
                                                unsigned char dflt)
{
    const char   *str;
    unsigned char tmp = dflt;
    const char   *dflt_str;

    if (dflt) {
        dflt_str = "yes";
    } else {
        dflt_str = "no";
    }
    str = qt_internal_get_env_str(envariable, dflt_str);
    if (str && *str) {
        char *errptr;
        tmp = (strtoul(str, &errptr, 0) != 0);
        if (*errptr != 0) {
            if (!strcasecmp(str, "yes") || !strcasecmp(str, "true")) {
                tmp = 1;
            } else if (!strcasecmp(str, "no") || !strcasecmp(str, "false")) {
                tmp = 0;
            } else {
                fprintf(stderr, "unparsable %s (%s)\n", envariable, str);
                tmp = dflt;
            }
        }
        qthread_debug(CORE_DETAILS, "envariable %s parsed as %s\n", envariable, tmp?"TRUE":"FALSE");
    }
    return tmp;
}

int INTERNAL qt_internal_unset_envstr(const char *envariable)
{
    char        mod_envariable[100];
    const char *str;

    check_info();

    snprintf(mod_envariable, 100, "QT_%s", envariable);
    str = getenv(mod_envariable);
    qthread_debug(CORE_BEHAVIOR, "checking envariable %s\n", envariable);
    if (str && *str) {
        if (info) {
            printf("%s = %s\n", mod_envariable, str);
        }
        return unsetenv(mod_envariable);
    } else {
        snprintf(mod_envariable, 100, "QTHREAD_%s", envariable);
        str = getenv(mod_envariable);
        if (str && *str) {
            if (info) {
                printf("%s = %s\n", mod_envariable, str);
            }
            return unsetenv(mod_envariable);
        }
    }
    if (info) {
        printf("[QT|QTHREAD]_%s = (unset)\n", envariable);
    }
    return 0;
}

/* vim:set expandtab: */

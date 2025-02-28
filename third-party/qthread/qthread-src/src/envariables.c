#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>  /* for fprintf() */
#include <stdlib.h> /* for getenv() and strtoul() */
#include <string.h>
#include <strings.h> /* for strcasecmp() */

#include "qt_asserts.h"
#include "qt_envariables.h"
#include "qt_visibility.h"

static int info = -1;

static char const *qt_internal_envstr(char const *envariable,
                                      char const *dflt) {
  char mod_envariable[100];
  char const *str;

  snprintf(mod_envariable, 100, "QT_%s", envariable);
  str = getenv(mod_envariable);
  if (str && *str) {
    if (info) { printf("%s = %s\n", mod_envariable, str); }
    return str;
  } else {
    snprintf(mod_envariable, 100, "QTHREAD_%s", envariable);
    str = getenv(mod_envariable);
    if (str && *str) {
      if (info) { printf("%s = %s\n", mod_envariable, str); }
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

static inline void check_info(void) {
  if (-1 == info) {
    char const *str;
    str = getenv("QT_INFO");
    if (str == NULL) { str = getenv("QTHREAD_INFO"); }
    if (str != NULL) {
      int tmp;
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

char const INTERNAL *qt_internal_get_env_str(char const *envariable,
                                             char const *dflt) {
  assert(strlen(envariable) < 90);
  check_info();
  return qt_internal_envstr(envariable, dflt);
}

unsigned long INTERNAL qt_internal_get_env_num(char const *envariable,
                                               unsigned long dflt,
                                               unsigned long zerodflt) {
  char const *str;
  unsigned long tmp = dflt;

  if (dflt != 0) {
    char dflt_str[21];

    snprintf(dflt_str, 21, "%lu", dflt);
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
    if (tmp == 0) { tmp = zerodflt; }
  }
  return tmp;
}

unsigned char INTERNAL qt_internal_get_env_bool(char const *envariable,
                                                unsigned char dflt) {
  char const *str;
  unsigned char tmp = dflt;
  char const *dflt_str;

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
  }
  return tmp;
}

int INTERNAL qt_internal_unset_envstr(char const *envariable) {
  char mod_envariable[100];
  char const *str;

  check_info();

  snprintf(mod_envariable, 100, "QT_%s", envariable);
  str = getenv(mod_envariable);
  if (str && *str) {
    if (info) { printf("%s = %s\n", mod_envariable, str); }
    return unsetenv(mod_envariable);
  } else {
    snprintf(mod_envariable, 100, "QTHREAD_%s", envariable);
    str = getenv(mod_envariable);
    if (str && *str) {
      if (info) { printf("%s = %s\n", mod_envariable, str); }
      return unsetenv(mod_envariable);
    }
  }
  if (info) { printf("[QT|QTHREAD]_%s = (unset)\n", envariable); }
  return 0;
}

/* vim:set expandtab: */

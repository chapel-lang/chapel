
#pragma once



struct dirent {
	char	d_name[256];
};

static inline int scandir(const char *dirp, struct dirent ***namelist,
	    int (*filter)(const struct dirent *),
	    int (*compar)(const struct dirent **, const struct dirent **))
{
	errno = ENOSYS;
	return -1;
}

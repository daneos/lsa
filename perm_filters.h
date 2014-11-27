/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL 2.0 license
 */

#if !defined(__PERM_FILTERS_H__)
#define __PERM_FILTERS_H__

#include <sys/types.h>
#include <sys/stat.h>

//-----------------------------------------------------------------------------
inline int filter_perm(const struct stat *f, mode_t mask);

//-----------------------------------------------------------------------------
inline int filter_perm(const struct stat *f, mode_t mask)
{
	if(mask == NULL) return 1;
	return f->st_mode & mask;
}

//-----------------------------------------------------------------------------
#endif /* __PERM_FILTERS_H__ */
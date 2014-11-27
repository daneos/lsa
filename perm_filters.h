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
int filter_none(const struct stat *finfo);

//-----------------------------------------------------------------------------
int filter_none(const struct stat *finfo)
{
	return 1;
}

//-----------------------------------------------------------------------------
#endif /* __PERM_FILTERS_H__ */
/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__PERM_FILTERS_H__)
#define __PERM_FILTERS_H__

#include <sys/types.h>
#include <sys/stat.h>

#include "options.h"

#define mask(_mode, _mask) ((_mode & _mask) == _mask)

//-----------------------------------------------------------------------------
int filter_perm(const struct stat *f, config *c);

//-----------------------------------------------------------------------------
#endif /* __PERM_FILTERS_H__ */
/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__FMODE_H__)
#define __FMODE_H__

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define GB 1073741824
#define MB 1048576
#define KB 1024
//-----------------------------------------------------------------------------
char *fperm(mode_t mode);
char ftype(mode_t mode);
char *fsize(off_t size);

//-----------------------------------------------------------------------------
#endif /* __FMODE_H__ */
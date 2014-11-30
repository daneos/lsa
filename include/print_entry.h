/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__PRINT_ENTRY_H__)
#define __PRINT_ENTRY_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <unistd.h>

#include "fmode.h"

//-----------------------------------------------------------------------------
void print_long(const struct stat *f, const char *fname, const char *d_name);

//-----------------------------------------------------------------------------
#endif /* __PRINT_ENTRY_H__ */ 
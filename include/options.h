/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__OPTIONS_H__)
#define __OPTIONS_H__

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <pwd.h>
#include <string.h>

#define OPT_OK		0
#define OPT_ERROR	1

//-----------------------------------------------------------------------------
typedef struct _rwx {
	int r : 1;
	int w : 1;
	int x : 1;
} rwx;

typedef struct _config {
	rwx p;
	int uid;
	int gids[NGROUPS_MAX];
	int gcount;
	char dir[PATH_MAX+1];
} config;

//-----------------------------------------------------------------------------
int options(int argc, char **argv, config *c);
void print_help(char *name);

//-----------------------------------------------------------------------------
#endif /* __OPTIONS_H__ */
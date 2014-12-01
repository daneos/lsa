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
#include <grp.h>
#include <string.h>
#include <sys/types.h>

#define OPT_OK		0
#define OPT_ERROR	1

#define VIEW_LONG	0
#define VIEW_SHORT	1
#define VIEW_MINI	2

//-----------------------------------------------------------------------------
typedef struct _rwx {
	int r : 1;
	int w : 1;
	int x : 1;
} rwx;

typedef struct _config {
	rwx p;
	uid_t uid;
	gid_t gids[NGROUPS_MAX];
	int gcount;
	char dir[PATH_MAX+1];
	short view;
} config;

//-----------------------------------------------------------------------------
int options(int argc, char **argv, config *c);
void print_help(char *name);

//-----------------------------------------------------------------------------
#endif /* __OPTIONS_H__ */
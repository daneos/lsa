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
int options(int argc, char **argv, config *c)
{
	int o, count;
	struct passwd *user;
	memset(c, 0, sizeof c);
	// set dir, uid and gids to current, should get overwritten if other specified
	strncpy(c->dir, ".", sizeof c->dir);
	c->uid = getuid();
	if((count = getgroups(0, NULL)) == -1)
	{
		perror("options()/getgroups()");
		return OPT_ERROR;
	}
	if((c->gcount = getgroups(count, c->gids)) == -1)
	{
		perror("options()/getgroups()");
		return OPT_ERROR;
	}
	
	// if no options passed set defaults
	if(argc == 1)
	{
		c->p.r = 1;
		c->p.w = 1;
		return OPT_OK;
	}

	opterr = 0;
	while((o = getopt(argc, argv, "rwxu:h")) != -1)
		switch(o)
		{
			case 'r':
				c->p.r = 1;
				break;
			case 'w':
				c->p.w = 1;
				break;
			case 'x':
				c->p.x = 1;
				break;
			case 'u':
				if((user = getpwnam(optarg)) == NULL)
				{
					perror("options()/getpwnam()");
					return OPT_ERROR;
				}
				c->uid = user->pw_uid;
				/*if((count = getgroupsbyname(optarg, 0, NULL)) == -1)
				{
					perror("options()/getgroupsbyname()");
					return OPT_ERROR;
				}
				if((c->gcount = getgroupsbyname(optarg, count, c->gids)) == -1)
				{
					perror("options()/getgroupsbyname()");
					return OPT_ERROR;
				}*/
				break;
			case 'h':
				print_help(argv[0]);
				break;
			
			case '?':
				if(optopt == 'u')
					fprintf(stderr, "Please provide username for -%c\n", optopt);
				else if(isprint (optopt))
					fprintf(stderr, "Option -%c not understood.\n", optopt);
				else
					fprintf(stderr, "Option -\\x%x not understood.\n", optopt);
			default:
				return OPT_ERROR;
		}
	if(argc > optind)
		strncpy(c->dir, argv[optind], sizeof c->dir);		

	return OPT_OK;
}

//-----------------------------------------------------------------------------
void print_help(char *name)
{
	printf("Copyright (C) 2014 daneos.\nReleased under the GNU GPL v2 license.\n\n"
		   "List only these files, you have access to\nUSAGE:\n"
		   "   %s [-rwx] [-u username] [dir]\n\n"
		   "   -rwx            which permissions shoud be checked (defaults to rw)\n"
		   "   -u username     check permissions for specific user (defaults to current user)\n"
		   "   dir             directory to list (defaults to .)\n\n"
	, name);
	exit(0);
}

//-----------------------------------------------------------------------------
#endif /* __OPTIONS_H__ */
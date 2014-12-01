/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "include/options.h"
#include "include/version.h"

//-----------------------------------------------------------------------------
int options(int argc, char **argv, config *c)
{
	int o, count = 0, norwx = 1;
	struct passwd *user;
	gid_t *gids = c->gids;
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

	opterr = 0;
	while((o = getopt(argc, argv, "rwxu:v:h")) != -1)
		switch(o)
		{
			case 'r':
				c->p.r = 1;
				norwx = 0;
				break;
			case 'w':
				c->p.w = 1;
				norwx = 0;
				break;
			case 'x':
				c->p.x = 1;
				norwx = 0;
				break;
			case 'u':
				if((user = getpwnam(optarg)) == NULL)
				{
					perror("options()/getpwnam()");
					return OPT_ERROR;
				}
				c->uid = user->pw_uid;
				getgrouplist(optarg, user->pw_gid, c->gids, &count);
				if((c->gcount = getgrouplist(optarg, user->pw_gid, c->gids, &count)) == -1)
				{
					perror("options()/getgrouplist()");
					return OPT_ERROR;
				}
				break;
			case 'v':
				switch(optarg[0])
				{
					case 'l':
						c->view = VIEW_LONG;
						break;
					case 's':
						c->view = VIEW_SHORT;
						break;
					case 'm':
						c->view = VIEW_MINI;
						break;
					default:
						fprintf(stderr, "-v: not recognized view: %c\n", optarg[0]);
						return OPT_ERROR;
				}
				break;
			case 'h':
				print_help(argv[0]);
				break;
			
			case '?':
				if(optopt == 'u')
					fprintf(stderr, "-%c: please provide username\n", optopt);
				else if(optopt == 'v')
					fprintf(stderr, "-%c: please provide view type\n", optopt);
				else if(isprint (optopt))
					fprintf(stderr, "-%c: not understood\n", optopt);
				else
					fprintf(stderr, "-\\x%x: not understood\n", optopt);
			default:
				return OPT_ERROR;
		}
	if(argc > optind)
		strncpy(c->dir, argv[optind], sizeof c->dir);

	// if no rwx flags passed, set to default
	if(norwx)
	{
		c->p.r = 1;
		c->p.w = 1;
	}

	return OPT_OK;
}

//-----------------------------------------------------------------------------
void print_help(char *name)
{
	printf("Version: %s\nCopyright (C) 2014 daneos.\nReleased under the GNU GPL v2 license.\n\n"
		   "List only these files, you have access to\nUSAGE:\n"
		   "   %s [-rwx] [-u username] [-v s/l/m] [dir]\n\n"
		   "   -rwx          which permissions shoud be checked (default: rw)\n"
		   "   -u username   check permissions for specific user (default: current user)\n"
		   "   -v s/l/m      s - short view, l - long view, m - minimal view (default: l)\n"
		   "   dir           directory to list (default: .)\n\n"
	, __VERSION, name);
	exit(0);
}
/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "include/perm_filters.h"

//-----------------------------------------------------------------------------
int filter_perm(const struct stat *f, config *c)
{
	int i;
	int rmask = (c->p.r ? 1<<2 : 0);
	int wmask = (c->p.w ? 1<<1 : 0);
	int xmask = (c->p.x ? 1 : 0);

	// check owner bits only if user is an owner
	if(f->st_uid == c->uid)
		if(mask(f->st_mode, rmask<<6) && mask(f->st_mode, wmask<<6) && mask(f->st_mode, xmask<<6)) return 1;

	// check group bits only if user is in file's group 
	for(i=0; i < c->gcount; i++)
		if(f->st_gid == c->gids[i])
			if(mask(f->st_mode, rmask<<3) && mask(f->st_mode, wmask<<3) && mask(f->st_mode, xmask<<3)) return 1;

	// always check other bits (unless access is granted from any of previous checks)
	if(mask(f->st_mode, rmask) && mask(f->st_mode, wmask) && mask(f->st_mode, xmask)) return 1;

	// all checks failed, user cannot acces the file
	return 0; 
}
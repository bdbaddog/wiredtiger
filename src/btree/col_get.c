/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2008-2011 WiredTiger, Inc.
 *	All rights reserved.
 */

#include "wt_internal.h"

/*
 * __wt_db_col_get --
 *	Db.col_get method.
 */
int
__wt_db_col_get(WT_TOC *toc, uint64_t recno, DBT *data)
{
	DB *db;
	BTREE *btree;
	int ret;

	db = toc->db;
	btree = db->btree;

	/* Search the column-store for the key. */
	if (!F_ISSET(btree, WT_COLUMN)) {
		__wt_api_db_errx(db,
		    "row-store records cannot be retrieved by record number");
		return (WT_ERROR);
	}

	WT_ERR(__wt_col_search(toc, recno, 0));
	ret = __wt_dbt_return(toc, NULL, data, 0);

err:	if (toc->srch_page != btree->root_page.page)
		__wt_hazard_clear(toc, toc->srch_page);
	return (ret);
}

//
// Copyright 2020 Staysail Systems, Inc. <info@staysail.tech>
// Copyright 2018 Capitar IT Group BV <info@capitar.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#ifndef CORE_IDHASH_H
#define CORE_IDHASH_H

#include "core/defs.h"

// We find that we often want to have a list of things listed by a
// numeric ID, which is generally monotonically increasing.  This is
// most often a pipe ID.  To help keep collections of these things
// indexed by their ID (which might start from a very large value),
// we offer a hash table.  The hash table uses open addressing, but
// we use a better probe (taken from Python) to avoid hitting the same
// positions.  Our hash algorithm is just the low order bits, and we
// use table sizes that are powers of two.  Note that hash items
// must be non-NULL.  The table is protected by an internal lock.

typedef struct nni_id_map   nni_id_map;
typedef struct nni_id_entry nni_id_entry;

// NB: These details are entirely private to the hash implementation.
// They are provided here to facilitate inlining in structures.
struct nni_id_map {
	size_t        id_cap;
	size_t        id_count;
	size_t        id_load;
	size_t        id_min_load; // considers placeholders
	size_t        id_max_load;
	uint32_t      id_min_val;
	uint32_t      id_max_val;
	uint32_t      id_dyn_val;
	nni_id_entry *id_entries;
};

extern void nni_id_map_init(nni_id_map *, uint32_t, uint32_t, bool);
extern void nni_id_map_fini(nni_id_map *);
extern void *nni_id_get(nni_id_map *, uint32_t);
extern int   nni_id_set(nni_id_map *, uint32_t, void *);
extern int   nni_id_alloc(nni_id_map *, uint32_t *, void *);
extern int   nni_id_remove(nni_id_map *, uint32_t);

// NanoMQ
void *       nni_id_get_any(nni_id_map *m, uint16_t *pid);
extern void  nni_id_msgfree_cb(nni_msg *msg);
extern void  nni_id_show_cb(nni_msg *msg);
extern void  nni_id_iterate(nni_id_map *m, void(func_cb)(nni_msg *));
// TODO duplicate with the last one API ^^
extern void  nni_id_map_foreach(nni_id_map *, nni_cb);
extern void *nni_id_get_one(nni_id_map *m, uint32_t *key);

#endif // CORE_IDHASH_H

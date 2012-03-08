/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2011, 2012       Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <common/config.h>
#include <starpu.h>
#include <common/utils.h>
#include <core/workers.h>
#include <math.h>
#include <sched_policies/detect_combined_workers.h>

#ifdef STARPU_HAVE_HWLOC
#include <hwloc.h>

/* struct _starpu_tree
 * ==================
 * Purpose
 * =======
 * Structure representing a tree (which can be a sub-tree itself) whose root is an hwloc
 * object and storing every workers it contained in every sub-trees by recursion.
 *
 * Fields
 * ======
 * obj			A hwloc object which can be a root or a leaf, it may be a numa node, a cache memory or a CPU, etc...
 *
 * nb_workers		Number of CPU workers which can be found by recursion in all the sub-trees beneath this one
 			or in this very object.
 *
 * workers		CPU-workers found by recursion in all the sub-trees and in this very one, represented as leaves in hwloc.
 */

struct _starpu_tree
{
    hwloc_obj_t obj;
    unsigned nb_workers;
    int *workers;
};

/* gather_trees
 * ============
 * Purpose
 * =======
 * Gather all the workers of every source tree in one target tree.
 * We assume the target array of workers is big enough to contain all the workers.
 *
 * Arguments
 * =========
 * target_tree		(input, output)
 *			Pointer to the tree which will contain all the workers of every source.
 *
 * source_trees		(input)
 *			Array of trees we want to combine in a unique tree.
 *
 * nb_source_trees	(input)
 *			Number of trees we want to combine (size of the array).
 */

static void gather_trees(struct _starpu_tree *target_tree, struct _starpu_tree *source_trees, unsigned nb_source_trees)
{
    unsigned tree_id, worker_id, index = 0;
    for(tree_id = 0; tree_id < nb_source_trees; ++tree_id)
	for(worker_id = 0; worker_id < source_trees[tree_id].nb_workers; ++worker_id)
	    target_tree->workers[index++] = source_trees[tree_id].workers[worker_id];
}

/* assign_multiple_trees
 * ========================
 * Purpose
 * =======
 * Assign every tree which is large enough (greater than min_size) and merge small ones.
 * If there is no tree large enough to be assigned any more, we return.
 *
 * Return value
 * ============
 * The number of workers assigned during the function.
 *
 * Arguments
 * =========
 * trees		(input, output)
 *			In entry, array of trees to assign. In the end at most one tree still contains workers.
 *
 * nb_trees		(input)
 *			The number of trees (size of the array).
 *
 * min_size		(input)
 *			Minimum size of a combined worker.
 *
 * max_size		(input)
 *			Maximum size of a combined worker.
 */

static unsigned assign_multiple_trees(struct _starpu_tree *trees, unsigned nb_trees, unsigned int min_size, unsigned int max_size)
{
    unsigned short complete = 0;
    unsigned tree_id, tree_id2, nb_workers_tree, nb_workers_tree2, worker_id, nb_workers_total = 0, nb_workers_assigned = 0;

    for(tree_id = 0; tree_id < nb_trees; ++tree_id)
	nb_workers_total += trees[tree_id].nb_workers;;

    while(!complete)
    {
	complete = 1;

	/* First we manage to assign every subtree large enough to be assigned alone */
	for(tree_id = 0; tree_id < nb_trees; ++tree_id)
	{
	    if(trees[tree_id].nb_workers== 0) // An already assigned subtree
		continue;

	    nb_workers_tree = trees[tree_id].nb_workers;

	    /* We shouldn't assign a small tree if we could assign the whole trees instead */
	    if(nb_workers_tree >= min_size && nb_workers_total > max_size)
	    {
		int ret = starpu_combined_worker_assign_workerid(nb_workers_tree, trees[tree_id].workers);
		STARPU_ASSERT(ret >= 0);
		nb_workers_assigned += nb_workers_tree;
		nb_workers_total -= nb_workers_tree;
		trees[tree_id].nb_workers = 0;
	    }
	}

	/* Then we merge too small subtrees into not too large ones
	 * if we manage to merge some subtrees we turn the flag
	 * complete to 0 thus we know he have to start again to assign
	 * just merged subtrees */
	for(tree_id = 0; tree_id < nb_trees; ++tree_id)
	{
	    if(trees[tree_id].nb_workers == 0) // An already assigned subtree
		continue;

	    nb_workers_tree = trees[tree_id].nb_workers;

	    /* We go through the array to find another subtree we can merge with this one */
	    for(tree_id2 = 0; tree_id2 < nb_trees; ++tree_id2)
	    {
		if(trees[tree_id2].nb_workers == 0 || tree_id == tree_id2) // An already assigned subtree or the same
		    continue;

		nb_workers_tree2 = trees[tree_id2].nb_workers;

		/*  We can merge the two subtrees, let's do it */
		if(nb_workers_tree + nb_workers_tree2 <= max_size)
		{
		    for(worker_id = 0; worker_id < nb_workers_tree2; ++worker_id)
			trees[tree_id].workers[nb_workers_tree + worker_id] = trees[tree_id2].workers[worker_id];

		    trees[tree_id].nb_workers += nb_workers_tree2;
		    trees[tree_id2].nb_workers = 0;

		    /* We just merged two subtrees, we need to restart again and try to assign it */
		    complete = 0;
		    break;
		}
	    }

	    if(!complete)
		break;
	}
    }

    return nb_workers_assigned;
}

/* find_and_assign_combinations_with_hwloc_recursive
 * =================================================
 * Purpose
 * =======
 * Go through the tree given as parameter and try to assign them. Workers it didn't succeed to
 * assign are given back to the calling function to be assigned using data from other subtrees if so.
 *
 * Return value
 * ============
 * The number of workers left to be assigned.
 *
 * Arguments
 * =========
 * tree			(input, output)
 *			Tree structure containing the root to process in entry.
 *			When the function returns it also contains the number of workers left
 *			to be assigned and these very workers in the array previously allocated.
 *
 * min_size		(input)
 *			Minimum size of a combined worker.
 *
 * max_size		(input)
 *			Maximum size of a combined worker.
 */

static unsigned find_and_assign_combinations_with_hwloc_recursive(struct _starpu_tree *tree, unsigned int min_size, unsigned int max_size)
{
    unsigned subtree_id, nb_workers = 0;

    hwloc_obj_t obj = tree->obj;
    int *workers = tree->workers;

    struct _starpu_machine_config *config = _starpu_get_machine_config();

    /* Is this a leaf ? (eg. a PU for hwloc) */
    if (!hwloc_compare_types(config->cpu_depth, obj->depth))
    {
	struct _starpu_worker *worker = obj->userdata;

	/* If this is a CPU worker add it at the beginning
	 * of the array , write 1 in the field nb_workers and
	 * return the number of CPU workers found : 1 in this case. */
	if (worker && worker->arch == STARPU_CPU_WORKER)
	{
	    workers[0] = worker->workerid;
	    tree->nb_workers = 1;
	    return 1;
	}

	tree->nb_workers = 0;
	return 0;
    }


    /* If there is only one child, we go to the next level right away */
    if (obj->arity == 1)
    {
	struct _starpu_tree subtree = *tree;
	subtree.obj = obj->children[0];
	nb_workers = find_and_assign_combinations_with_hwloc_recursive(&subtree, min_size, max_size);
	tree->nb_workers = nb_workers;
	return nb_workers;
    }

    /* We recursively go to the leaves of the tree to find subtrees which have the biggest number of
     * CPU leaves that fits between min and max. */

    /* We allocate an array of tree structures which will contain the current node's subtrees data */
    struct _starpu_tree *subtrees = (struct _starpu_tree *) malloc(obj->arity * sizeof(struct _starpu_tree));

    /* We allocate the array containing the workers of each subtree and initialize the fields left */
    for(subtree_id = 0; subtree_id < obj->arity; ++subtree_id)
    {
	struct _starpu_tree *subtree = subtrees + subtree_id;

	subtree->obj = obj->children[subtree_id];
	subtree->nb_workers = 0;
	subtree->workers = (int *) malloc(config->topology.nhwcpus * sizeof(int));
    }

    /* We recursively go through every subtree and get all the workers which are not assigned yet */
    for(subtree_id = 0; subtree_id < obj->arity; ++subtree_id)
	nb_workers += find_and_assign_combinations_with_hwloc_recursive(subtrees + subtree_id, min_size, max_size);

    if(nb_workers > max_size)
    {
	/* We withdraw the number of workers just assigned from the total number of workers */
	nb_workers -= assign_multiple_trees(subtrees, obj->arity, min_size, max_size);

	/* Some workers are not assigned yet : we gather them in the array
	 * which is returned to the father which will handle them later */
	if(nb_workers)
	    gather_trees(tree, subtrees, obj->arity);
    }
    else if(nb_workers < max_size)
    {
	gather_trees(tree, subtrees, obj->arity);
    }
    else // nb_workers == max_size
    {
	gather_trees(tree, subtrees, obj->arity);

	int ret = starpu_combined_worker_assign_workerid(nb_workers, workers);
	STARPU_ASSERT(ret >= 0);
	nb_workers = 0;
    }

    for(subtree_id = 0; subtree_id < obj->arity; ++subtree_id)
	free(subtrees[subtree_id].workers);
    free(subtrees);

    tree->nb_workers = nb_workers;
    return nb_workers;
}

/* get_min_max_sizes
 * =================================================
 * Purpose
 * =======
 * First, try to get the value from the STARPU_MIN_WORKERSIZE and STARPU_MAX_WORKERSIZE
 * environment variables.
 * If both of them were not set, then we try do get some efficient values following the rule beneath :
 *
 * 				-->   exact 	-->  MIN_SIZE = S-1 <--> MAX_SIZE = S+1
 * S = square_root(nb_cpus)
 *				-->   decimal 	-->  MIN_SIZE = truncation(S) <--> MAX_SIZE = rounding_up(S)
 *
 * If only one of both was not set then we set it with a value relative to the other, for example :
 *
 *		 	MIN_SIZE = MAX_SIZE - 1 or MAX_SIZE = MIN_SIZE + 1
 *
 * Arguments
 * =========
 * min_size		(output)
 *			Pointer to the minimum size of a combined worker, whether set with
 *			value given by the user or processed from the number of cpus.
 *
 * max_size		(output)
 *			Pointer to the maximum size of a combined worker, whether set with
 *			value given by the user or processed from the number of cpus.
 *
 * topology		(input)
 *			Topology of the machine : used to know the number of cpus.
 */

static void get_min_max_sizes(unsigned int *min_size, unsigned int *max_size, struct starpu_machine_topology *topology)
{
    int _min_size, _max_size;

    _min_size = starpu_get_env_number("STARPU_MIN_WORKERSIZE");
    _max_size = starpu_get_env_number("STARPU_MAX_WORKERSIZE");

    /* If the user didn't set both the environment variables,
     * we need to find a minimum and a maximum size ourselves */
    if(_min_size <= -1 || _max_size <= -1)
    {

	int nb_cpus = topology->nhwcpus;
	int sqrt_nb_cpus = (int)sqrt((double)nb_cpus);
	int exact = (sqrt_nb_cpus * sqrt_nb_cpus == nb_cpus);

	    if(_min_size == -1)
	    {
		if(_max_size > -1)
		    _min_size = _max_size - 1;
		else
		    _min_size = exact ? sqrt_nb_cpus - 1 : sqrt_nb_cpus;
	    }

	if(_max_size == -1)
	{
	    if(_min_size > -1)
		_max_size = _min_size + 1;
	    else
		_max_size = sqrt_nb_cpus + 1;
	}
    }

    *min_size = _min_size;
    *max_size = _max_size;

    return;
}

/* find_and_assign_combinations_with_hwloc
 * =======================================
 * Purpose
 * =======
 * Launches find_and_assign_combinations_with_hwloc_recursive function on the root
 * of the hwloc tree to gather and assign combined cpu workers in an efficient manner.
 * When find_and_assign_combinations_with_hwloc_recursive returns, if there are still
 * some workers, we assign them no matter the number for there is no way to respect
 * the wanted sizes anymore.
 *
 * Arguments
 * =========
 * topology		(input)
 *			Topology of the machine : used to know the number of cpus and
 *			to get the hwloc tree.
 */

static void find_and_assign_combinations_with_hwloc(struct starpu_machine_topology *topology)
{
    unsigned nb_workers;
    unsigned int min_size, max_size;

    get_min_max_sizes(&min_size, &max_size, topology);

    STARPU_ASSERT(min_size <= max_size);

    struct _starpu_tree tree;

    /* Of course we start from the root */
    tree.obj = hwloc_get_obj_by_depth(topology->hwtopology, HWLOC_OBJ_SYSTEM, 0);
    tree.nb_workers = 0;
    tree.workers = (int *) malloc(topology->nhwcpus * sizeof(int));

    /* We recursively go from the root to the leaves of the tree to find
     * subtrees that only have CPUs as leaves. */
    nb_workers = find_and_assign_combinations_with_hwloc_recursive(&tree, min_size, max_size);

    /* There are still some workers left, since the only possibility is that
     * the number of workers left is less than the minimum worker size we assign them all */
    if(nb_workers > 0)
    {
	/* find_and_assign_combinations_with_hwloc_recursive shouldn't return
	 * while there are enough workers to assign regarding the min_size value */
	STARPU_ASSERT(nb_workers <= max_size);

	int ret = starpu_combined_worker_assign_workerid(nb_workers, tree.workers);
	STARPU_ASSERT(ret >= 0);
    }

    free(tree.workers);
}

#else /* STARPU_HAVE_HWLOC */

static void find_and_assign_combinations_without_hwloc(struct starpu_machine_topology *topology)
{
    struct _starpu_machine_config *config = _starpu_get_machine_config();

    /* We put the id of all CPU workers in this array */
    int cpu_workers[STARPU_NMAXWORKERS];
    unsigned ncpus = 0;

    unsigned i;
    for (i = 0; i < topology->nworkers; i++)
    {
	if (config->workers[i].perf_arch == STARPU_CPU_DEFAULT)
	    cpu_workers[ncpus++] = i;
    }

    unsigned size;
    for (size = 2; size <= ncpus; size *= 2)
    {
	unsigned first_cpu;
	for (first_cpu = 0; first_cpu < ncpus; first_cpu += size)
	{
	    if (first_cpu + size <= ncpus)
	    {
		int workerids[size];

		for (i = 0; i < size; i++)
		    workerids[i] = cpu_workers[first_cpu + i];

		/* We register this combination */
		int ret;
		ret = starpu_combined_worker_assign_workerid(size, workerids);
		STARPU_ASSERT(ret >= 0);
	    }
	}
    }
}

#endif /* STARPU_HAVE_HWLOC */

static void combine_all_cpu_workers(struct starpu_machine_topology *topology)
{
    struct _starpu_machine_config *config = _starpu_get_machine_config();

    int cpu_workers[STARPU_NMAXWORKERS];
    unsigned ncpus = 0;

    unsigned i;
    for (i = 0; i < topology->nworkers; i++)
    {
	if (config->workers[i].perf_arch == STARPU_CPU_DEFAULT)
	    cpu_workers[ncpus++] = i;
    }

    for (i = 1; i <= ncpus; i++)
    {
	int ret;
	ret = starpu_combined_worker_assign_workerid(i, cpu_workers);
	STARPU_ASSERT(ret >= 0);
    }
}

void _starpu_sched_find_worker_combinations(struct starpu_machine_topology *topology)
{
    struct _starpu_machine_config *config = _starpu_get_machine_config();

    if ((config->user_conf && config->user_conf->single_combined_worker > 0) || starpu_get_env_number("STARPU_SINGLE_COMBINED_WORKER") > 0)
	combine_all_cpu_workers(topology);
    else
    {
#ifdef STARPU_HAVE_HWLOC
	find_and_assign_combinations_with_hwloc(topology);
#else
	find_and_assign_combinations_without_hwloc(topology);
#endif
    }
}

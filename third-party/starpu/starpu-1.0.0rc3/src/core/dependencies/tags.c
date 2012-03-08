/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

#include <starpu.h>
#include <common/config.h>
#include <common/utils.h>
#include <core/dependencies/tags.h>
#include <core/dependencies/htable.h>
#include <core/jobs.h>
#include <core/sched_policy.h>
#include <core/dependencies/data_concurrency.h>
#include <profiling/bound.h>

static struct _starpu_htbl_node *tag_htbl = NULL;
static pthread_rwlock_t tag_global_rwlock = PTHREAD_RWLOCK_INITIALIZER;

static struct _starpu_cg *create_cg_apps(unsigned ntags)
{
	struct _starpu_cg *cg = (struct _starpu_cg *) malloc(sizeof(struct _starpu_cg));
	STARPU_ASSERT(cg);

	cg->ntags = ntags;
	cg->remaining = ntags;
	cg->cg_type = STARPU_CG_APPS;

	cg->succ.succ_apps.completed = 0;
	_STARPU_PTHREAD_MUTEX_INIT(&cg->succ.succ_apps.cg_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&cg->succ.succ_apps.cg_cond, NULL);

	return cg;
}

static struct _starpu_cg *create_cg_tag(unsigned ntags, struct _starpu_tag *tag)
{
	struct _starpu_cg *cg = (struct _starpu_cg *) malloc(sizeof(struct _starpu_cg));
	STARPU_ASSERT(cg);

	cg->ntags = ntags;
	cg->remaining = ntags;
	cg->cg_type = STARPU_CG_TAG;

	cg->succ.tag = tag;
	tag->tag_successors.ndeps++;

	return cg;
}

static struct _starpu_tag *_starpu_tag_init(starpu_tag_t id)
{
	struct _starpu_tag *tag;
	tag = (struct _starpu_tag *) malloc(sizeof(struct _starpu_tag));
	STARPU_ASSERT(tag);

	tag->job = NULL;
	tag->is_assigned = 0;
	tag->is_submitted = 0;

	tag->id = id;
	tag->state = STARPU_INVALID_STATE;

	_starpu_cg_list_init(&tag->tag_successors);

	_starpu_spin_init(&tag->lock);

	return tag;
}

static void _starpu_tag_free(void *_tag)
{
	struct _starpu_tag *tag = (struct _starpu_tag *) _tag;

	if (tag) {
		_starpu_spin_lock(&tag->lock);

		unsigned nsuccs = tag->tag_successors.nsuccs;
		unsigned succ;

		for (succ = 0; succ < nsuccs; succ++)
		{
			struct _starpu_cg *cg = tag->tag_successors.succ[succ];

			unsigned ntags = STARPU_ATOMIC_ADD(&cg->ntags, -1);
			unsigned remaining __attribute__ ((unused)) = STARPU_ATOMIC_ADD(&cg->remaining, -1);

			if (!ntags && (cg->cg_type == STARPU_CG_TAG))
				/* Last tag this cg depends on, cg becomes unreferenced */
				free(cg);
		}

#ifdef STARPU_DYNAMIC_DEPS_SIZE
		free(tag->tag_successors.succ);
#endif

		_starpu_spin_unlock(&tag->lock);
		_starpu_spin_destroy(&tag->lock);

		free(tag);
	}
}

void starpu_tag_remove(starpu_tag_t id)
{
	struct _starpu_tag *tag;

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&tag_global_rwlock);

	tag = (struct _starpu_tag *) _starpu_htbl_remove_tag(&tag_htbl, id);

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&tag_global_rwlock);

	_starpu_tag_free(tag);
}

void _starpu_tag_clear(void)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&tag_global_rwlock);

	/* XXX: _starpu_tag_free takes the tag spinlocks while we are keeping
	 * the global rwlock. This contradicts the lock order of
	 * starpu_tag_wait_array. Should not be a problem in practice since
	 * _starpu_tag_clear is called at shutdown only. */
	_starpu_htbl_clear_tags(&tag_htbl, 0, _starpu_tag_free);

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&tag_global_rwlock);
}

static struct _starpu_tag *gettag_struct(starpu_tag_t id)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&tag_global_rwlock);

	/* search if the tag is already declared or not */
	struct _starpu_tag *tag;
	tag = (struct _starpu_tag *) _starpu_htbl_search_tag(tag_htbl, id);

	if (tag == NULL)
	{
		/* the tag does not exist yet : create an entry */
		tag = _starpu_tag_init(id);

		void *old;
		old = _starpu_htbl_insert_tag(&tag_htbl, id, tag);
		/* there was no such tag before */
		STARPU_ASSERT(old == NULL);
	}

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&tag_global_rwlock);

	return tag;
}

/* lock should be taken */
void _starpu_tag_set_ready(struct _starpu_tag *tag)
{
	/* mark this tag as ready to run */
	tag->state = STARPU_READY;
	/* declare it to the scheduler ! */
	struct _starpu_job *j = tag->job;

	/* In case the task job is going to be scheduled immediately, and if
	 * the task is "empty", calling _starpu_push_task would directly try to enforce
	 * the dependencies of the task, and therefore it would try to grab the
	 * lock again, resulting in a deadlock. */
	_starpu_spin_unlock(&tag->lock);

	/* enforce data dependencies */
	_starpu_enforce_deps_starting_from_task(j);

	_starpu_spin_lock(&tag->lock);
}

/* the lock must be taken ! */
static void _starpu_tag_add_succ(struct _starpu_tag *tag, struct _starpu_cg *cg)
{
	STARPU_ASSERT(tag);

	_starpu_add_successor_to_cg_list(&tag->tag_successors, cg);

	if (tag->state == STARPU_DONE)
	{
		/* the tag was already completed sooner */
		_starpu_notify_cg(cg);
	}
}

void _starpu_notify_tag_dependencies(struct _starpu_tag *tag)
{
	_starpu_spin_lock(&tag->lock);

	if (tag->state == STARPU_DONE) {
		_starpu_spin_unlock(&tag->lock);
		return;
	}

	tag->state = STARPU_DONE;
	_STARPU_TRACE_TAG_DONE(tag);

	_starpu_notify_cg_list(&tag->tag_successors);

	_starpu_spin_unlock(&tag->lock);
}

void starpu_tag_notify_from_apps(starpu_tag_t id)
{
	struct _starpu_tag *tag = gettag_struct(id);

	_starpu_notify_tag_dependencies(tag);
}

void _starpu_tag_declare(starpu_tag_t id, struct _starpu_job *job)
{
	_STARPU_TRACE_TAG(id, job);
	job->task->use_tag = 1;

	struct _starpu_tag *tag= gettag_struct(id);
	tag->job = job;
	tag->is_assigned = 1;

	job->tag = tag;

	/* the tag is now associated to a job */
	_starpu_spin_lock(&tag->lock);
	tag->state = STARPU_ASSOCIATED;
	_starpu_spin_unlock(&tag->lock);
}

void starpu_tag_declare_deps_array(starpu_tag_t id, unsigned ndeps, starpu_tag_t *array)
{
	unsigned i;

	/* create the associated completion group */
	struct _starpu_tag *tag_child = gettag_struct(id);

	_starpu_spin_lock(&tag_child->lock);
	struct _starpu_cg *cg = create_cg_tag(ndeps, tag_child);
	_starpu_spin_unlock(&tag_child->lock);

	STARPU_ASSERT(ndeps != 0);

	for (i = 0; i < ndeps; i++)
	{
		starpu_tag_t dep_id = array[i];

		/* id depends on dep_id
		 * so cg should be among dep_id's successors*/
		_STARPU_TRACE_TAG_DEPS(id, dep_id);
		_starpu_bound_tag_dep(id, dep_id);
		struct _starpu_tag *tag_dep = gettag_struct(dep_id);
		STARPU_ASSERT(tag_dep != tag_child);
		_starpu_spin_lock(&tag_dep->lock);
		_starpu_spin_lock(&tag_child->lock);
		_starpu_tag_add_succ(tag_dep, cg);
		_starpu_spin_unlock(&tag_child->lock);
		_starpu_spin_unlock(&tag_dep->lock);
	}
}

void starpu_tag_declare_deps(starpu_tag_t id, unsigned ndeps, ...)
{
	unsigned i;

	/* create the associated completion group */
	struct _starpu_tag *tag_child = gettag_struct(id);

	_starpu_spin_lock(&tag_child->lock);
	struct _starpu_cg *cg = create_cg_tag(ndeps, tag_child);
	_starpu_spin_unlock(&tag_child->lock);

	STARPU_ASSERT(ndeps != 0);

	va_list pa;
	va_start(pa, ndeps);
	for (i = 0; i < ndeps; i++)
	{
		starpu_tag_t dep_id;
		dep_id = va_arg(pa, starpu_tag_t);

		/* id depends on dep_id
		 * so cg should be among dep_id's successors*/
		_STARPU_TRACE_TAG_DEPS(id, dep_id);
		_starpu_bound_tag_dep(id, dep_id);
		struct _starpu_tag *tag_dep = gettag_struct(dep_id);
		STARPU_ASSERT(tag_dep != tag_child);
		_starpu_spin_lock(&tag_dep->lock);
		_starpu_spin_lock(&tag_child->lock);
		_starpu_tag_add_succ(tag_dep, cg);
		_starpu_spin_unlock(&tag_child->lock);
		_starpu_spin_unlock(&tag_dep->lock);
	}
	va_end(pa);
}

/* this function may be called by the application (outside callbacks !) */
int starpu_tag_wait_array(unsigned ntags, starpu_tag_t *id)
{
	unsigned i;
	unsigned current;

	struct _starpu_tag *tag_array[ntags];

	_STARPU_LOG_IN();

	/* It is forbidden to block within callbacks or codelets */
	if (STARPU_UNLIKELY(!_starpu_worker_may_perform_blocking_calls()))
	{
		_STARPU_LOG_OUT_TAG("edeadlk");
		return -EDEADLK;
	}

	/* only wait the tags that are not done yet */
	for (i = 0, current = 0; i < ntags; i++)
	{
		struct _starpu_tag *tag = gettag_struct(id[i]);

		_starpu_spin_lock(&tag->lock);

		if (tag->state == STARPU_DONE)
		{
			/* that tag is done already */
			_starpu_spin_unlock(&tag->lock);
		}
		else
		{
			tag_array[current] = tag;
			current++;
		}
	}

	if (current == 0)
	{
		/* all deps are already fulfilled */
		_STARPU_LOG_OUT_TAG("all deps are already fulfilled");
		return 0;
	}

	/* there is at least one task that is not finished */
	struct _starpu_cg *cg = create_cg_apps(current);

	for (i = 0; i < current; i++)
	{
		_starpu_tag_add_succ(tag_array[i], cg);
		_starpu_spin_unlock(&tag_array[i]->lock);
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&cg->succ.succ_apps.cg_mutex);

	while (!cg->succ.succ_apps.completed)
		_STARPU_PTHREAD_COND_WAIT(&cg->succ.succ_apps.cg_cond, &cg->succ.succ_apps.cg_mutex);

	_STARPU_PTHREAD_MUTEX_UNLOCK(&cg->succ.succ_apps.cg_mutex);

	_STARPU_PTHREAD_MUTEX_DESTROY(&cg->succ.succ_apps.cg_mutex);
	_STARPU_PTHREAD_COND_DESTROY(&cg->succ.succ_apps.cg_cond);

	free(cg);

	_STARPU_LOG_OUT();
	return 0;
}

int starpu_tag_wait(starpu_tag_t id)
{
	return starpu_tag_wait_array(1, &id);
}

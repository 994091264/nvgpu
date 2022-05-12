/*
 * Copyright (c) 2021-2022 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#include <nvs/log.h>
#include <nvs/sched.h>
#include <nvs/domain.h>

/*
 * Create a new domain and populate it with default values
 */
struct nvs_domain *nvs_domain_create(struct nvs_sched *sched,
		  const char *name, u64 timeslice, u64 preempt_grace,
		  void *priv)
{
	struct nvs_domain *dom = nvs_malloc(sched, sizeof(*dom));

	nvs_log(sched, "Creating domain - %s", name);

	if (dom == NULL) {
		return NULL;
	}

	nvs_memset(dom, 0, sizeof(*dom));

	strncpy(dom->name, name, sizeof(dom->name) - 1);
	dom->timeslice_ns     = timeslice;
	dom->preempt_grace_ns = preempt_grace;
	dom->priv             = priv;
	dom->next             = NULL;

	return dom;
}

struct nvs_domain *nvs_domain_get_next_domain(struct nvs_sched *sched, struct nvs_domain *dom)
{
	struct nvs_domain *nvs_next = dom->next;

	if (nvs_next == NULL) {
		nvs_next = sched->domain_list->domains;
	}

	return nvs_next;
}

/*
 * Attach a new domain to the end of the domain list.
 */
void nvs_domain_scheduler_attach(struct nvs_sched *sched, struct nvs_domain *dom)
{
	struct nvs_domain_list *dlist = sched->domain_list;

	/*
	 * Now add the domain to the list of domains. If this is the first
	 * domain we are done. Otherwise use the last pointer to quickly
	 * append the domain.
	 */
	dlist->nr++;
	if (dlist->domains == NULL) {
		dlist->domains = dom;
		dlist->last    = dom;
		return;
	}

	dlist->last->next = dom;
	dlist->last       = dom;

	nvs_log(sched, "%s: Domain added", dom->name);
}

/*
 * Unlink the domain from our list and clear the last pointer if this was the
 * only one remaining.
 */
static void nvs_domain_unlink(struct nvs_sched *sched,
			      struct nvs_domain *dom)
{
	struct nvs_domain_list *dlist = sched->domain_list;
	struct nvs_domain *tmp;

	if (dlist->domains == dom) {
		dlist->domains = dom->next;

		/*
		 * If dom == last and dom is the first entry, then we have a
		 * single entry in the list and we need to clear the last
		 * pointer as well.
		 */
		if (dom == dlist->last) {
			dlist->domains = NULL;
			dlist->last = NULL;
		}
		return;
	}

	nvs_domain_for_each(sched, tmp) {
		/*
		 * If tmp's next pointer is dom, then we take tmp and have it
		 * skip over dom. But also don't forget to handle last; if dom
		 * is last, then last becomes the one before it.
		 */
		if (dom == tmp->next) {
			tmp->next = dom->next;
			if (dom == dlist->last) {
				dlist->last = tmp;
			}
			return;
		}
	}
}

void nvs_domain_destroy(struct nvs_sched *sched,
			struct nvs_domain *dom)
{
	nvs_memset(dom, 0, sizeof(*dom));
	nvs_free(sched, dom);
}

void nvs_domain_unlink_and_destroy(struct nvs_sched *sched,
			struct nvs_domain *dom)
{
	nvs_log_event(sched, NVS_EV_REMOVE_DOMAIN, 0);

	nvs_domain_unlink(sched, dom);
	sched->domain_list->nr--;

	nvs_domain_destroy(sched, dom);
}

void nvs_domain_clear_all(struct nvs_sched *sched)
{
	struct nvs_domain_list *dlist = sched->domain_list;

	while (dlist->domains != NULL) {
		nvs_domain_unlink_and_destroy(sched, dlist->domains);
	}
}

u32 nvs_domain_count(struct nvs_sched *sched)
{
	return sched->domain_list->nr;
}

struct nvs_domain *nvs_domain_by_name(struct nvs_sched *sched, const char *name)
{
	struct nvs_domain *domain;

	nvs_domain_for_each(sched, domain) {
		if (strcmp(domain->name, name) == 0) {
			return domain;
		}
	}

	return NULL;
}

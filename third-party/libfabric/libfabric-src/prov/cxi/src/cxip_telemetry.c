/*
 * Copyright (c) 2022 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */
#include "config.h"
#include "cxip.h"

#define TELEMETRY_FILE_FMT "/sys/class/cxi/cxi%u/device/telemetry/%s"

static long cxip_telemetry_entry_read_value(struct cxip_telemetry_entry *entry)
{
	long ret;
	char path[FI_PATH_MAX];
	FILE *f;
	long value;
	long tstamp_sec;
	long tstamp_nsec;

	ret = snprintf(path, FI_PATH_MAX, TELEMETRY_FILE_FMT,
		       entry->telemetry->dom->iface->info->dev_id, entry->name);
	if (ret < 0)
		return ret;

	f = fopen(path, "r");
	if (!f)
		return -errno;

	/* Even though only value is needed, extract 3 values to ensure
	 * telemetry data is in the expected format.
	 */
	ret = fscanf(f, "%ld@%ld.%ld", &value, &tstamp_sec, &tstamp_nsec);
	if (ret != 3) {
		if (ret == EOF)
			ret = -errno;
		else
			ret = -FI_EINVAL;
	} else {
		ret = value;
	}

	fclose(f);

	return ret;
}

static void cxip_telemetry_entry_dump_delta(struct cxip_telemetry_entry *entry)
{
	long delta;

	delta = cxip_telemetry_entry_read_value(entry);
	if (delta < 0) {
		DOM_WARN(entry->telemetry->dom, "Failed to read %s: %ld:%s\n",
			 entry->name, delta, fi_strerror(-delta));
		return;
	}

	if (delta < entry->value) {
		DOM_WARN(entry->telemetry->dom,
			 "Failed to perform delta due to %s reset\n",
			 entry->name);
		return;
	}

	delta -= entry->value;

	DOM_INFO(entry->telemetry->dom, "%s: %ld\n", entry->name, delta);
}

static int cxip_telemetry_entry_reset_value(struct cxip_telemetry_entry *entry)
{
	long ret;

	ret = cxip_telemetry_entry_read_value(entry);
	if (ret < 0) {
		DOM_WARN(entry->telemetry->dom, "Failed to read %s: %ld:%s\n",
			 entry->name, ret, fi_strerror(-ret));
		return ret;
	}

	entry->value = ret;

	return FI_SUCCESS;
}

static void
cxip_telemetry_entry_free(struct cxip_telemetry_entry *entry)
{
	dlist_remove(&entry->telemetry_entry);
	free(entry);
}

static bool
cxip_telemetry_entry_validate_token_file(struct cxip_telemetry *telemetry,
					 const char *telemetry_token)
{
	char path[FI_PATH_MAX];
	int ret;

	ret = snprintf(path, FI_PATH_MAX, TELEMETRY_FILE_FMT,
		       telemetry->dom->iface->info->dev_id, telemetry_token);
	if (ret < 0)
		return false;

	/* Verify user read access to the telemetry file. */
	if (access(path, R_OK))
		return false;

	return true;
}

static bool
cxip_telemetry_entry_validate_token(struct cxip_telemetry *telemetry,
				    const char *telemetry_token)
{
	/* The telemetry directory has an ALL-in-binary entry. This file is
	 * considered invalid for this telemetry implementation.
	 */
	if (strcmp(telemetry_token, "ALL-in-binary") == 0)
		return false;

	return cxip_telemetry_entry_validate_token_file(telemetry,
							telemetry_token);
}

static int cxip_telemetry_entry_alloc(struct cxip_telemetry *telemetry,
				      const char *telemetry_token)
{
	struct cxip_telemetry_entry *entry;
	int ret;

	if (!cxip_telemetry_entry_validate_token(telemetry, telemetry_token)) {
		DOM_WARN(telemetry->dom, "Invalid telemetry: %s\n",
			 telemetry_token);
		return -FI_EINVAL;
	}

	entry = calloc(1, sizeof(*entry));
	if (!entry)
		return -FI_ENOMEM;

	entry->telemetry = telemetry;

	strncpy(entry->name, telemetry_token, TELEMETRY_ENTRY_NAME_SIZE - 1);
	entry->name[TELEMETRY_ENTRY_NAME_SIZE - 1] = '\0';

	/* Revalidate the name after the memcpy. */
	if (!cxip_telemetry_entry_validate_token(telemetry, entry->name)) {
		DOM_WARN(telemetry->dom, "Invalid telemetry: %s\n",
			 entry->name);
		ret = FI_EINVAL;
		goto err_free_entry;
	}

	ret = cxip_telemetry_entry_reset_value(entry);
	if (ret)
		goto err_free_entry;

	dlist_insert_tail(&entry->telemetry_entry, &telemetry->telemetry_list);

	return FI_SUCCESS;

err_free_entry:
	free(entry);

	return ret;
}

static int cxip_telemetry_sleep_duration(void)
{
	int ret;
	int msec_sleep;
	char *path = "/sys/module/cxi_core/parameters/cntr_refresh_interval";
	FILE *f;

	f = fopen(path, "r");
	if (!f)
		return -errno;

	ret = fscanf(f, "%d", &msec_sleep);
	if (ret != 1) {
		if (ret == EOF)
			ret = -errno;
		else
			ret = -FI_EINVAL;
	} else {
		/* Convert sleep duration to seconds. */
		ret = msec_sleep / 1000;
		if (msec_sleep % 1000)
			ret++;
		ret = MAX(ret, 1);
	}

	fclose(f);

	return ret;
}

void cxip_telemetry_dump_delta(struct cxip_telemetry *telemetry)
{
	struct cxip_telemetry_entry *entry;
	int sleep_duration;

	/* Since sysfs telemetry entries are refreshed as some interval, we need
	 * to sleep for a refresh interval to get updates. Else, the application
	 * could run and telemetry deltas would be zero.
	 */
	sleep_duration = cxip_telemetry_sleep_duration();
	if (sleep_duration < 0) {
		DOM_WARN(telemetry->dom,
			 "Failed to retrieve telemetry sleep duration: %d:%s\n",
			 sleep_duration, fi_strerror(-sleep_duration));
		return;
	}

	sleep(sleep_duration);

	dlist_foreach_container(&telemetry->telemetry_list,
				struct cxip_telemetry_entry, entry,
				telemetry_entry)
		cxip_telemetry_entry_dump_delta(entry);
}

void cxip_telemetry_free(struct cxip_telemetry *telemetry)
{
	struct cxip_telemetry_entry *entry;
	struct dlist_entry *tmp;

	dlist_foreach_container_safe(&telemetry->telemetry_list,
				     struct cxip_telemetry_entry,
				     entry, telemetry_entry, tmp)
		cxip_telemetry_entry_free(entry);

	free(telemetry);
}

int cxip_telemetry_alloc(struct cxip_domain *dom,
			 struct cxip_telemetry **telemetry)
{
	struct cxip_telemetry *_telemetry;
	const char *telemetry_token;
	char *telemetry_copy;
	int ret = FI_SUCCESS;

	/* If user defined telemtry string is NULL or RGID does not match,
	 * return -FI_ENOSYS signalling this function is not supported.
	 */
	if (!cxip_env.telemetry ||
	    (cxip_env.telemetry_rgid >= 0 &&
	     dom->lni->lni->id != cxip_env.telemetry_rgid))
		return -FI_ENOSYS;

	_telemetry = calloc(1, sizeof(*_telemetry));
	if (!_telemetry)
		return -FI_ENOMEM;

	_telemetry->dom = dom;
	dlist_init(&_telemetry->telemetry_list);

	telemetry_copy = malloc(strlen(cxip_env.telemetry) + 1);
	if (!telemetry_copy) {
		ret = -FI_ENOMEM;
		goto err_free_telemetry;
	}

	strcpy(telemetry_copy, cxip_env.telemetry);

	/* The following will parse the comma separated list and attempt to
	 * allocate a telemetry entry for any valid substring/token. If a
	 * telemetry entry fails to be allocated for a given substring/token,
	 * this is not considered fatal and parsing will continue.
	 */
	telemetry_token = strtok(telemetry_copy, ",");
	while (telemetry_token != NULL) {
		ret = cxip_telemetry_entry_alloc(_telemetry, telemetry_token);
		if (ret)
			DOM_WARN(dom, "Failed to allocated %s telemetry entry: %d:%s\n",
				 telemetry_token, ret, fi_strerror(-ret));
		else
			DOM_INFO(dom, "Telemetry entry allocated for %s\n",
				 telemetry_token);

		telemetry_token = strtok(NULL, ",");
	}

	free(telemetry_copy);

	if (dlist_empty(&_telemetry->telemetry_list)) {
		DOM_WARN(dom, "Failed to allocated any telemetry entries\n");
		ret = -FI_EINVAL;
		goto err_free_telemetry;
	}

	*telemetry = _telemetry;

	return FI_SUCCESS;

err_free_telemetry:
	cxip_telemetry_free(_telemetry);

	return ret;
}

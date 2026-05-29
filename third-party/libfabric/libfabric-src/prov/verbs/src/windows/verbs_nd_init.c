/*
 * Copyright (c) 2022 Canon Medical Research USA, Inc.  All rights reserved.
 * Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <assert.h>
#include <initguid.h>
#include <guiddef.h>

#include <ws2spi.h>
#include "ndspi.h"
#include "verbs_nd.h"

/* Adapters must be sorted by nd_adapter::address. */
static size_t nd_remove_dups(struct nd_adapter *adapters, size_t num)
{
	struct nd_adapter *dst = adapters;
	struct nd_adapter *src = adapters + 1;

	size_t i;
	size_t n = 1;

	if (num < 2)
		return num;

	for (i = 1; i < num; i++) {
		if (!ofi_equals_ipaddr(&dst->address.addr,
				       &src->address.addr)) {
			++dst;
			if (dst != src)
				memcpy(dst, src, sizeof(struct nd_adapter));
			n++;
		}
		++src;
	}

	return n;
}

/*
 * nd_strerror generates a string message based on err value (GetLastError).
 * Returned string is valid until the next call of nd_strerror
 */
static char *nd_strerror(DWORD err, HMODULE module)
{
	static char *message = NULL;
	size_t size;

	/* if message is allocated - free it */
	if (message)
		LocalFree(message);

	size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			(module ? FORMAT_MESSAGE_FROM_HMODULE : 0),
		module, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&message, 0, NULL);

	return size ? message : (char *)"";
}

static char *nd_get_last_error_str(HRESULT hr, char *errmsg,
				   SIZE_T max_msg_len)
{
	LPVOID lpMsgBuf;
	DWORD dw = (DWORD)hr;
	errno_t rc;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			      FORMAT_MESSAGE_FROM_SYSTEM |
			      FORMAT_MESSAGE_IGNORE_INSERTS,
		      NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		      (LPTSTR)&lpMsgBuf, 0, NULL);

	strcpy_s(errmsg, max_msg_len, "NTStatus: ");
	rc = strncat_s(errmsg, max_msg_len - strlen(errmsg), lpMsgBuf,
		       _TRUNCATE);

	LocalFree(lpMsgBuf);

	return errmsg;
}

#define NDERR(err, str)                                                        \
	case err:                                                              \
		str = #err;                                                    \
		break

char *nd_error_str(HRESULT hr)
{
	static char lerr[128];
	char *err_str = NULL;

	switch (hr) {
	NDERR(ND_SUCCESS, err_str);
	NDERR(ND_TIMEOUT, err_str);
	NDERR(ND_PENDING, err_str);
	NDERR(ND_BUFFER_OVERFLOW, err_str);
	NDERR(ND_DEVICE_BUSY, err_str);
	NDERR(ND_NO_MORE_ENTRIES, err_str);
	NDERR(ND_UNSUCCESSFUL, err_str);
	NDERR(ND_ACCESS_VIOLATION, err_str);
	NDERR(ND_INVALID_HANDLE, err_str);
	NDERR(ND_INVALID_DEVICE_REQUEST, err_str);
	NDERR(ND_INVALID_PARAMETER, err_str);
	NDERR(ND_NO_MEMORY, err_str);
	NDERR(ND_INVALID_PARAMETER_MIX, err_str);
	NDERR(ND_DATA_OVERRUN, err_str);
	NDERR(ND_SHARING_VIOLATION, err_str);
	NDERR(ND_INSUFFICIENT_RESOURCES, err_str);
	NDERR(ND_DEVICE_NOT_READY, err_str);
	NDERR(ND_IO_TIMEOUT, err_str);
	NDERR(ND_NOT_SUPPORTED, err_str);
	NDERR(ND_INTERNAL_ERROR, err_str);
	NDERR(ND_INVALID_PARAMETER_1, err_str);
	NDERR(ND_INVALID_PARAMETER_2, err_str);
	NDERR(ND_INVALID_PARAMETER_3, err_str);
	NDERR(ND_INVALID_PARAMETER_4, err_str);
	NDERR(ND_INVALID_PARAMETER_5, err_str);
	NDERR(ND_INVALID_PARAMETER_6, err_str);
	NDERR(ND_INVALID_PARAMETER_7, err_str);
	NDERR(ND_INVALID_PARAMETER_8, err_str);
	NDERR(ND_INVALID_PARAMETER_9, err_str);
	NDERR(ND_INVALID_PARAMETER_10, err_str);
	NDERR(ND_CANCELED, err_str);
	NDERR(ND_REMOTE_ERROR, err_str);
	NDERR(ND_INVALID_ADDRESS, err_str);
	NDERR(ND_INVALID_DEVICE_STATE, err_str);
	NDERR(ND_INVALID_BUFFER_SIZE, err_str);
	NDERR(ND_TOO_MANY_ADDRESSES, err_str);
	NDERR(ND_ADDRESS_ALREADY_EXISTS, err_str);
	NDERR(ND_CONNECTION_REFUSED, err_str);
	NDERR(ND_CONNECTION_INVALID, err_str);
	NDERR(ND_CONNECTION_ACTIVE, err_str);
	NDERR(ND_HOST_UNREACHABLE, err_str);
	NDERR(ND_CONNECTION_ABORTED, err_str);
	NDERR(ND_DEVICE_REMOVED, err_str);
	NDERR(ND_DISCONNECTED, err_str);
	default:
		err_str = nd_get_last_error_str(hr, lerr, sizeof(lerr));
		if (err_str == NULL) {
			_snprintf(lerr, sizeof(lerr), "Unknown ND error %#08ld",
				  hr);
			err_str = lerr;
		}
		break;
	}
	return err_str;
}

struct nd_infrastructure nd_infra = { 0 };

static void nd_free_infra()
{
	size_t i;
	int ret = 0;
	struct nd_adapter *adapter = NULL;
	struct nd_factory *factory = NULL;
	struct nd_module *module = NULL;

	if (nd_infra.adapters.count) {
		assert(nd_infra.adapters.adapter);
		for (i = 0; i < nd_infra.adapters.count; i++) {
			adapter = &nd_infra.adapters.adapter[i];
			if (adapter->adapter) {
				// I'd like to have the following code, but cannot since it fails
				// because the ND provider dll has already been unloaded.
				//   ret = adapter->adapter->lpVtbl->Release(adapter->adapter);
				//   assert(!ret);
				ret = adapter->factory->provider->lpVtbl->Release(
					adapter->factory->provider);
				assert(ret == 1);
				adapter->adapter = NULL;
			}
			if (adapter->name) {
				free((void *)adapter->name);
				adapter->name = NULL;
			}
		}
		free(nd_infra.adapters.adapter);
		free(nd_infra.adapters.adapter_list);
		nd_infra.adapters.adapter = NULL;
		nd_infra.adapters.adapter_list = NULL;
		nd_infra.adapters.device_list = NULL;
		nd_infra.adapters.context_list = NULL;
		nd_infra.adapters.count = 0;
	}

	if (nd_infra.class_factories.count) {
		assert(nd_infra.class_factories.factory);
		for (i = 0; i < nd_infra.class_factories.count; i++) {
			factory = &nd_infra.class_factories.factory[i];
			if (factory->provider) {
				ret = factory->provider->lpVtbl->Release(
					factory->provider);
				assert(!ret);
				factory->provider = NULL;
			}
			if (factory->class_factory) {
				ret = factory->class_factory->lpVtbl->Release(
					factory->class_factory);
				assert(!ret);
				factory->class_factory = NULL;
			}
			assert(factory->module);
			if (factory->addr_list) {
				free(factory->addr_list);
				factory->addr_list = NULL;
			}
		}
		free(nd_infra.class_factories.factory);
		nd_infra.class_factories.factory = NULL;
		nd_infra.class_factories.count = 0;
	}

	if (nd_infra.providers.count) {
		assert(nd_infra.providers.modules);
		for (i = 0; i < nd_infra.providers.count; i++) {
			module = &nd_infra.providers.modules[i];
			assert(module->path);
			free((void *)module->path);
		}
		free(nd_infra.providers.modules);
		nd_infra.providers.modules = NULL;
		nd_infra.providers.count = 0;
	}
}

static HRESULT nd_alloc_infra(size_t cnt)
{
	memset(&nd_infra, 0, sizeof(*(&nd_infra)));

	nd_infra.providers.modules =
		malloc(cnt * sizeof(*nd_infra.providers.modules));
	if (!nd_infra.providers.modules)
		return ND_NO_MEMORY;

	nd_infra.class_factories.factory =
		malloc(cnt * sizeof(*nd_infra.class_factories.factory));
	if (!nd_infra.class_factories.factory) {
		nd_free_infra();
		return ND_NO_MEMORY;
	}

	return S_OK;
}

static wchar_t *nd_get_provider_path(const WSAPROTOCOL_INFOW *proto)
{
	int len = 0, lenex, err, res;
	wchar_t *prov, *provex;

	assert(proto);

	res = WSCGetProviderPath((GUID *)&proto->ProviderId, NULL, &len, &err);
	if (err != WSAEFAULT || !len)
		return NULL;

	prov = malloc(len * sizeof(*prov));
	if (!prov)
		return NULL;

	res = WSCGetProviderPath((GUID *)&proto->ProviderId, prov, &len, &err);
	if (res)
		goto fn1;

	lenex = ExpandEnvironmentStringsW(prov, NULL, 0);
	if (!lenex)
		goto fn1;

	provex = malloc(lenex * sizeof(*provex));
	if (!provex)
		goto fn1;

	lenex = ExpandEnvironmentStringsW(prov, provex, lenex);
	if (!lenex)
		goto fn2;

	free(prov);
	return provex;

fn2:
	free(provex);
fn1:
	free(prov);
	return NULL;
}

#define ND_PROTO_FLAG                                                       \
	(XP1_GUARANTEED_DELIVERY | XP1_GUARANTEED_ORDER |                      \
	 XP1_MESSAGE_ORIENTED | XP1_CONNECT_DATA)

static int nd_is_valid_proto(const WSAPROTOCOL_INFOW *proto)
{
	assert(proto);

	if ((proto->dwServiceFlags1 & ND_PROTO_FLAG) != ND_PROTO_FLAG)
		return 0;

	if (!(proto->iAddressFamily == AF_INET ||
	      proto->iAddressFamily == AF_INET6))
		return 0;

	if (proto->iSocketType != -1)
		return 0;

	if (proto->iProtocol || proto->iProtocolMaxOffset)
		return 0;
	return 1;
}

static struct nd_module *nd_search_module(const wchar_t *path)
{
	size_t i;
	size_t j;

	for (i = 0; i < nd_infra.providers.count; i++) {
		if (path && nd_file_exists(path) &&
		    !nd_is_directory(path)) {
			for (j = 0; j < nd_infra.providers.count; j++) {
				if (nd_is_same_file(
					    path, nd_infra.providers.modules[j]
							  .path)) {
					return &nd_infra.providers.modules[j];
				}
			}
		}
	}
	return NULL;
}

static struct nd_module *nd_create_module(const wchar_t *path)
{
	struct nd_module *module;
	HMODULE hmodule;
	dll_can_unload_now unload;
	dll_get_class_object getclass;

	assert(nd_infra.providers.modules);

	module = nd_search_module(path);
	if (module)
		return module;

	/* ok, this is not duplicate. try to
	load it and get class factory*/
	hmodule = LoadLibraryW(path);
	if (!hmodule) {
		VRB_WARN(
			FI_LOG_CORE,
			"nd_create_module: provider : %S, failed to load: %s\n",
			path, nd_strerror(GetLastError(), 0));
		return NULL;
	}

	unload = (dll_can_unload_now)GetProcAddress(hmodule, "DllCanUnloadNow");
	getclass = (dll_get_class_object)GetProcAddress(hmodule,
							"DllGetClassObject");
	if (!unload || !getclass) {
		VRB_WARN(
			FI_LOG_CORE,
			"nd_create_module: provider: %S, failed to import interface\n",
			path);
		goto fn_noiface;
	}

	module = &nd_infra.providers.modules[nd_infra.providers.count];
	nd_infra.providers.count++;

	module->path = _wcsdup(path);
	module->module = hmodule;
	module->can_unload_now = unload;
	module->get_class_object = getclass;

	return module;

fn_noiface:
	FreeLibrary(hmodule);
	return NULL;
}

#define ND_GUID_FORMAT                                                         \
	"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"
#define ND_GUID_ARG(guid)                                                      \
	(guid).Data1, (guid).Data2, (guid).Data3, (guid).Data4[0],             \
		(guid).Data4[1], (guid).Data4[2], (guid).Data4[3],             \
		(guid).Data4[4], (guid).Data4[5], (guid).Data4[6],             \
		(guid).Data4[7]

static HRESULT nd_create_factory(const WSAPROTOCOL_INFOW *proto)
{
	wchar_t *path;
	struct nd_module *module;
	IClassFactory *factory;
	HRESULT hr;
	struct nd_factory *ftr;

	assert(proto);
	assert(nd_is_valid_proto(proto));
	assert(nd_infra.class_factories.factory);

	path = nd_get_provider_path(proto);
	if (path)
		VRB_INFO(FI_LOG_CORE,
			 "nd_create_factory: provider " ND_GUID_FORMAT
			 " path: %S \n",
			 ND_GUID_ARG(proto->ProviderId), path);
	else /* can't get provider path. just return */
		return S_OK;

	module = nd_create_module(path);
	free(path);
	if (!module)
		return S_OK;

	assert(module->get_class_object);
	hr = module->get_class_object(&proto->ProviderId, &IID_IClassFactory,
				      (void **)&factory);
	if (FAILED(hr))
		return hr;

	ftr = &nd_infra.class_factories.factory[nd_infra.class_factories.count];
	nd_infra.class_factories.count++;
	ftr->class_factory = factory;
	ftr->module = module;
	ftr->protocol = *proto;

	return S_OK;
}

static int nd_adapter_cmp(const void *adapter1, const void *adapter2)
{
	return nd_addr_cmp(&((struct nd_adapter *)adapter1)->address,
			   &((struct nd_adapter *)adapter2)->address);
}

static HRESULT nd_create_adapter(void)
{
	size_t addr_count = 0;
	HRESULT hr;
	struct nd_factory *factory = NULL;
	struct nd_adapter *adapter = NULL;
	ULONG listsize = 0;
	wchar_t *saddr;
	DWORD addrlen = 0;
	UINT64 id;
	int res;
	ULONG linfo = sizeof(adapter->info);
	ND2_ADAPTER_INFO *info = NULL;

	for (size_t i = 0; i < nd_infra.class_factories.count; i++) {
		factory = &nd_infra.class_factories.factory[i];
		assert(factory->class_factory);

		hr = factory->class_factory->lpVtbl->CreateInstance(
			factory->class_factory, NULL, &IID_IND2Provider,
			(void **)&factory->provider);
		if (FAILED(hr))
			goto err1;
		listsize = 0;
		hr = factory->provider->lpVtbl->QueryAddressList(
			factory->provider, NULL, &listsize);
		if (hr != ND_BUFFER_OVERFLOW)
			goto err1;
		if (!listsize) {
			factory->provider->lpVtbl->Release(factory->provider);
			factory->provider = NULL;
			continue;
		}

		factory->addr_list = malloc(listsize);
		if (!factory->addr_list) {
			hr = ND_NO_MEMORY;
			goto err1;
		}

		hr = factory->provider->lpVtbl->QueryAddressList(
			factory->provider, factory->addr_list, &listsize);
		if (FAILED(hr))
			goto err1;

		for (size_t j = 0; j < factory->addr_list->iAddressCount; j++) {
			if (nd_is_valid_addr(
				    factory->addr_list->Address[j].lpSockaddr))
				addr_count++;
		}
	}

	if (!addr_count) {
		hr = E_NOINTERFACE;
		goto err1;
	}

	nd_infra.adapters.adapter =
		malloc(addr_count * sizeof(*nd_infra.adapters.adapter));
	if (!nd_infra.adapters.adapter) {
		hr = ND_NO_MEMORY;
		goto err1;
	}

	/* put all available valid addresses into common array */
	for (size_t i = 0; i < nd_infra.class_factories.count; i++) {
		factory = &nd_infra.class_factories.factory[i];
		for (size_t j = 0; j < factory->addr_list->iAddressCount; j++) {
			if (nd_is_valid_addr(
				    factory->addr_list->Address[j].lpSockaddr)) {
				adapter = &nd_infra.adapters.adapter
						   [nd_infra.adapters.count];
				assert((int)sizeof(adapter->address) >=
				       factory->addr_list->Address[j]
					       .iSockaddrLength);
				memcpy(&adapter->address,
				       factory->addr_list->Address[j].lpSockaddr,
				       factory->addr_list->Address[j]
					       .iSockaddrLength);
				adapter->factory = factory;
				nd_infra.adapters.count++;
			}
		}
	}

	if (!nd_infra.adapters.count) {
		hr = E_NOINTERFACE;
		goto err2;
	}

	/* sort adapters by addresses to set IP4 addresses first. then remove
	   duplicates */
	qsort(nd_infra.adapters.adapter, nd_infra.adapters.count,
	      sizeof(struct nd_adapter), nd_addr_cmp);
	nd_infra.adapters.count = nd_remove_dups(nd_infra.adapters.adapter,
						 nd_infra.adapters.count);

	// Allocate memory for adapter_list, device_list, and context_list all at once.
	nd_infra.adapters.adapter_list =
		calloc(nd_infra.adapters.count + 1,
		       sizeof(*nd_infra.adapters.adapter_list) +
			       sizeof(*nd_infra.adapters.device_list) +
			       sizeof(*nd_infra.adapters.context_list));
	if (!nd_infra.adapters.adapter_list) {
		hr = ND_NO_MEMORY;
		goto err2;
	}

	nd_infra.adapters.device_list =
		(struct ibv_device **)&nd_infra.adapters
			.adapter_list[nd_infra.adapters.count + 1];
	nd_infra.adapters.context_list =
		(struct ibv_context **)&nd_infra.adapters
			.adapter_list[(nd_infra.adapters.count + 1) * 2];

	for (size_t i = 0; i < nd_infra.adapters.count; i++) {
		adapter = &nd_infra.adapters.adapter[i];
		nd_infra.adapters.adapter_list[i] = adapter;
		nd_infra.adapters.device_list[i] = &adapter->device;
		nd_infra.adapters.context_list[i] = &adapter->context;
		factory = adapter->factory;

		assert(factory);
		assert(factory->provider);

		assert(adapter->address.addr.sa_family == AF_INET ||
		       adapter->address.addr.sa_family == AF_INET6);

		hr = factory->provider->lpVtbl->ResolveAddress(
			factory->provider, &adapter->address.addr,
			(ULONG)ofi_sizeofaddr(&adapter->address.addr), &id);
		if (FAILED(hr))
			goto err2;

		hr = factory->provider->lpVtbl->OpenAdapter(
			factory->provider, &IID_IND2Adapter, id,
			(void **)&adapter->adapter);
		if (FAILED(hr))
			goto err2;

		adapter->info.InfoVersion = ND_VERSION_2;
		hr = adapter->adapter->lpVtbl->Query(adapter->adapter,
						     &adapter->info, &linfo);
		if (FAILED(hr) && hr == ND_BUFFER_OVERFLOW) {
			info = malloc(linfo);
			if (!info) {
				hr = ND_NO_MEMORY;
				goto err2;
			}
			info->InfoVersion = ND_VERSION_2;
			hr = adapter->adapter->lpVtbl->Query(adapter->adapter,
							     info, &linfo);
			if (!FAILED(hr))
				adapter->info = *info;
			free(info);
			if (FAILED(hr))
				goto err2;
		} else if (FAILED(hr))
			goto err2;

		/* generate adapter's name */
		addrlen = 0;
		res = WSAAddressToStringW(
			&adapter->address.addr,
			(ULONG)ofi_sizeofaddr(&adapter->address.addr), NULL,
			NULL, &addrlen);
		if (res == SOCKET_ERROR && WSAGetLastError() == WSAEFAULT &&
		    addrlen) {
			saddr = malloc((addrlen + 1) * sizeof(*saddr));
			if (!saddr) {
				hr = ND_NO_MEMORY;
				goto err2;
			}
			WSAAddressToStringW(
				&adapter->address.addr,
				(ULONG)ofi_sizeofaddr(&adapter->address.addr),
				NULL, saddr, &addrlen);
		} else {
			saddr = _wcsdup(L"unknown");
		}

		asprintf((char **)&adapter->name, "%S", saddr);
		free(saddr);

		adapter->device.transport_type = IBV_TRANSPORT_IB;
		strncpy(&adapter->device.name[0], adapter->name,
			min(strlen(adapter->name) + 1,
			    sizeof(adapter->device.name)));
		adapter->device.name[IBV_SYSFS_NAME_MAX - 1] = 0;

		adapter->context.device = &adapter->device;
		adapter->context.num_comp_vectors = 0;
	}

	return S_OK;

err2:
	if (!nd_infra.adapters.count)
		free(nd_infra.adapters.adapter);
err1:
	nd_free_infra();
	return hr;
}

static HRESULT nd_init()
{
	DWORD proto_len = 0;
	HRESULT hr = ND_INTERNAL_ERROR;
	int i, protonum, err;
	size_t prov_count = 0;
	WSAPROTOCOL_INFOW *proto = 0;
	int ret;

	memset(&nd_infra, 0, sizeof(nd_infra));

	ret = WSCEnumProtocols(NULL, NULL, &proto_len, &err);
	if (ret != SOCKET_ERROR || err != WSAENOBUFS) {
		hr = ND_NO_MEMORY;
		goto fn_exit;
	}

	proto = malloc(proto_len);
	if (!proto) {
		hr = ND_NO_MEMORY;
		goto fn_exit;
	}

	protonum = WSCEnumProtocols(NULL, proto, &proto_len, &err);
	if (protonum == SOCKET_ERROR) {
		hr = ND_INTERNAL_ERROR;
		goto fn_protofail;
	}

	/* calculating number of valid protocols. this number is used
	   as maximum of existing providers and class factories */
	for (i = 0; i < protonum; i++) {
		if (nd_is_valid_proto(&proto[i]))
			prov_count++;
	}

	if (!prov_count) {
		hr = E_NOINTERFACE;
		goto fn_protofail;
	}

	hr = nd_alloc_infra(prov_count);
	if (hr != S_OK)
		goto fn_protofail;

	for (i = 0; i < protonum; i++) {
		if (nd_is_valid_proto(&proto[i]))
			nd_create_factory(&proto[i]);
	}

	/* ok, factories are created, now list all available addresses, try to
	   create adapters & collect adapter's info */
	hr = nd_create_adapter();

fn_protofail:
	free(proto);
fn_exit:
	return hr;
}

static bool nd_startup_done = false;

/* We don't need exclusive execution here because this function
 * is called from OFI init routine which is single thread
 */
HRESULT nd_startup()
{
	WSADATA data;
	HRESULT hr;
	int ret;

	if (nd_startup_done)
		return S_OK;

	VRB_INFO(FI_LOG_CORE, "nd_startup: starting initialization\n");

	ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (ret)
		return HRESULT_FROM_WIN32(ret);

	VRB_INFO(FI_LOG_CORE, "nd_startup: WSAStartup complete\n");

	hr = nd_init();

	nd_startup_done = true;

	return hr;
}

void nd_shutdown()
{
	if (!nd_startup_done)
		return;

	VRB_INFO(FI_LOG_CORE, "nd_shutdown: shutdown WSA\n");

	nd_free_infra();
	nd_startup_done = false;

	WSACleanup();
}

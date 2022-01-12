/*
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

#ifdef _WIN32

#include <initguid.h>
#include <guiddef.h>

#include <ws2spi.h>
#include <assert.h>
#include "ndspi.h"

#include "netdir.h"
#include "netdir_log.h"

#ifndef ofi_sizeofaddr
#define ofi_sizeofaddr(address)			\
	(address)->sa_family == AF_INET ?	\
		sizeof(struct sockaddr_in) :	\
		sizeof(struct sockaddr_in6)
#endif

#define FI_ND_PROTO_FLAG (XP1_GUARANTEED_DELIVERY | XP1_GUARANTEED_ORDER | \
			  XP1_MESSAGE_ORIENTED | XP1_CONNECT_DATA)

static int ofi_nd_startup_done = 0;

typedef HRESULT(*can_unload_now_t)(void);
typedef HRESULT(*get_class_object_t)(REFCLSID rclsid, REFIID rrid, LPVOID* ppv);

struct module_t {
	const wchar_t		*path;
	HMODULE			module;
	can_unload_now_t	can_unload_now;
	get_class_object_t	get_class_object;
};

struct factory_t {
	WSAPROTOCOL_INFOW	protocol;
	IClassFactory		*class_factory;
	IND2Provider		*provider;
	struct module_t		*module;
	SOCKET_ADDRESS_LIST	*addr_list;
};

struct adapter_t {
	union {
		struct sockaddr		addr;
		struct sockaddr_in	addr4;
		struct sockaddr_in6	addr6;
	} address;
	ND2_ADAPTER_INFO		info;
	IND2Adapter			*adapter;
	struct factory_t		*factory;
	const char			*name;
};

static struct ofi_nd_infra_t {
	struct modules_t {
		struct module_t		*modules;
		size_t			count;
	} providers;

	struct class_factory_t {
		struct factory_t	*factory;
		size_t			count;
	} class_factories;

	struct adapters_t {
		struct adapter_t	*adapter;
		size_t			count;
	} adapters;
} ofi_nd_infra = {0};

/* release all objects, do not free strings or arrays */
static inline void ofi_nd_release_infra()
{
	size_t i;

	if (ofi_nd_infra.adapters.count) {
		assert(ofi_nd_infra.adapters.adapter);
		for (i = 0; i < ofi_nd_infra.adapters.count; i++) {
			struct adapter_t *adapter = &ofi_nd_infra.adapters.adapter[i];
			if (adapter->adapter) {
				adapter->adapter->lpVtbl->Release(adapter->adapter);
				adapter->adapter = 0;
			}
		}
	}

	if (ofi_nd_infra.class_factories.count) {
		assert(ofi_nd_infra.class_factories.factory);
		for (i = 0; i < ofi_nd_infra.class_factories.count; i++) {
			struct factory_t *factory = &ofi_nd_infra.class_factories.factory[i];
			if (factory->provider) {
				factory->provider->lpVtbl->Release(factory->provider);
				factory->provider = 0;
			}
			if (factory->class_factory) {
				factory->class_factory->lpVtbl->Release(factory->class_factory);
				factory->class_factory = 0;
			}
		}
	}
}

static inline void ofi_nd_free_infra()
{
	size_t i;

	ofi_nd_release_infra();

	if (ofi_nd_infra.adapters.count) {
		assert(ofi_nd_infra.adapters.adapter);
		for (i = 0; i < ofi_nd_infra.adapters.count; i++) {
			struct adapter_t *adapter = &ofi_nd_infra.adapters.adapter[i];
			if (adapter->name) {
				free((void*)adapter->name);
				adapter->name = 0;
			}
		}
		free(ofi_nd_infra.adapters.adapter);
		ofi_nd_infra.adapters.adapter = 0;
		ofi_nd_infra.adapters.count = 0;
	}

	if (ofi_nd_infra.class_factories.count) {
		assert(ofi_nd_infra.class_factories.factory);
		for (i = 0; i < ofi_nd_infra.class_factories.count; i++) {
			struct factory_t *factory = &ofi_nd_infra.class_factories.factory[i];
			assert(factory->module);
			if (factory->addr_list) {
				free(factory->addr_list);
				factory->addr_list = 0;
			}
		}
		free(ofi_nd_infra.class_factories.factory);
		ofi_nd_infra.class_factories.factory = 0;
		ofi_nd_infra.class_factories.count = 0;
	}

	if (ofi_nd_infra.providers.count) {
		assert(ofi_nd_infra.providers.modules);
		for (i = 0; i < ofi_nd_infra.providers.count; i++) {
			struct module_t *module = &ofi_nd_infra.providers.modules[i];
			assert(module->path);
			free((void*)module->path);
		}
		free(ofi_nd_infra.providers.modules);
		ofi_nd_infra.providers.modules = 0;
		ofi_nd_infra.providers.count = 0;
	}
}

static inline HRESULT ofi_nd_alloc_infra(size_t cnt)
{
	memset(&ofi_nd_infra, 0, sizeof(*(&ofi_nd_infra)));

	ofi_nd_infra.providers.modules = (struct module_t*)malloc(cnt * sizeof(*ofi_nd_infra.providers.modules));
	if (!ofi_nd_infra.providers.modules)
		return ND_NO_MEMORY;

	ofi_nd_infra.class_factories.factory = (struct factory_t*)malloc(cnt * sizeof(*ofi_nd_infra.class_factories.factory));
	if (!ofi_nd_infra.class_factories.factory) {
		ofi_nd_free_infra();
		return ND_NO_MEMORY;
	}

	return S_OK;
}

static inline wchar_t *ofi_nd_get_provider_path(const WSAPROTOCOL_INFOW *proto)
{
	assert(proto);

	int len, lenex, err, res;
	wchar_t *prov, *provex;

	res = WSCGetProviderPath((GUID*)&proto->ProviderId, NULL, &len, &err);
	if (err != WSAEFAULT || !len)
		return NULL;

	prov = (wchar_t*)malloc(len * sizeof(*prov));
	if (!prov)
		return NULL;

	res = WSCGetProviderPath((GUID*)&proto->ProviderId, prov, &len, &err);
	if (res)
		goto fn1;

	lenex = ExpandEnvironmentStringsW(prov, NULL, 0);
	if (!lenex)
		goto fn1;

	provex = (wchar_t*)malloc(lenex * sizeof(*provex));
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

static inline int ofi_nd_is_valid_proto(const WSAPROTOCOL_INFOW *proto)
{
	assert(proto);

	if ((proto->dwServiceFlags1 & FI_ND_PROTO_FLAG) != FI_ND_PROTO_FLAG)
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

static inline struct module_t *ofi_nd_search_module(const wchar_t* path)
{
	size_t i;
	size_t j;

	for (i = 0; i < ofi_nd_infra.providers.count; i++) {
		if (path && ofi_nd_file_exists(path) &&
		    !ofi_nd_is_directory(path)) {
			for (j = 0; j < ofi_nd_infra.providers.count; j++) {
				if (ofi_nd_is_same_file(path, ofi_nd_infra.providers.modules[j].path)) {
					return &ofi_nd_infra.providers.modules[j];
				}
			}
		}
	}
	return NULL;
}

static inline struct module_t *ofi_nd_create_module(const wchar_t* path)
{
	struct module_t *module;
	HMODULE hmodule;
	can_unload_now_t unload;
	get_class_object_t getclass;

	assert(ofi_nd_infra.providers.modules);

	module = ofi_nd_search_module(path);
	if (module)
		return module;

	/* ok, this is not duplicate. try to
	load it and get class factory*/
	hmodule = LoadLibraryW(path);
	if (!hmodule) {
		ND_LOG_WARN(FI_LOG_CORE,
			   "ofi_nd_create_module: provider : %S, failed to load: %s\n",
			   path, ofi_nd_strerror(GetLastError(), 0));
		return NULL;
	}

	unload = (can_unload_now_t)GetProcAddress(hmodule, "DllCanUnloadNow");
	getclass = (get_class_object_t)GetProcAddress(hmodule, "DllGetClassObject");
	if (!unload || !getclass) {
		ND_LOG_WARN(FI_LOG_CORE,
			   "ofi_nd_create_module: provider: %S, failed to import interface\n",
			   path);
		goto fn_noiface;
	}

	module = &ofi_nd_infra.providers.modules[ofi_nd_infra.providers.count];
	ofi_nd_infra.providers.count++;

	module->path = _wcsdup(path);
	module->module = hmodule;
	module->can_unload_now = unload;
	module->get_class_object = getclass;

	return module;

fn_noiface:
	FreeLibrary(hmodule);
	return NULL;
}

static inline HRESULT ofi_nd_create_factory(const WSAPROTOCOL_INFOW* proto)
{
	wchar_t *path;
	struct module_t *module;
	IClassFactory* factory;
	HRESULT hr;
	struct factory_t *ftr;

	assert(proto);
	assert(ofi_nd_is_valid_proto(proto));
	assert(ofi_nd_infra.class_factories.factory);

	path = ofi_nd_get_provider_path(proto);
	if (path)
		ND_LOG_INFO(FI_LOG_CORE,
			    "ofi_nd_create_factory: provider " FI_ND_GUID_FORMAT " path: %S \n",
			    FI_ND_GUID_ARG(proto->ProviderId), path);
	else /* can't get provider path. just return */
		return S_OK;

	module = ofi_nd_create_module(path);
	free(path);
	if (!module)
		return S_OK;

	assert(module->get_class_object);
	hr = module->get_class_object(&proto->ProviderId, &IID_IClassFactory,
				      (void**)&factory);
	if (FAILED(hr))
		return hr;

	ftr = &ofi_nd_infra.class_factories.factory[ofi_nd_infra.class_factories.count];
	ofi_nd_infra.class_factories.count++;
	ftr->class_factory = factory;
	ftr->module = module;
	ftr->protocol = *proto;

	return S_OK;
}

static int ofi_nd_adapter_cmp(const void *adapter1, const void *adapter2)
{
	return ofi_nd_addr_cmp(&((struct adapter_t*)adapter1)->address,
			       &((struct adapter_t*)adapter2)->address);
}

static HRESULT ofi_nd_create_adapter(void)
{
	size_t addr_count = 0;
	HRESULT hr;

	for (size_t i = 0; i < ofi_nd_infra.class_factories.count; i++) {
		struct factory_t *factory = &ofi_nd_infra.class_factories.factory[i];
		ULONG listsize = 0;

		assert(factory->class_factory);

		hr = factory->class_factory->lpVtbl->CreateInstance(factory->class_factory,
			NULL, &IID_IND2Provider, (void**)&factory->provider);
		if (FAILED(hr))
			return hr;
		hr = factory->provider->lpVtbl->QueryAddressList(factory->provider, NULL, &listsize);
		if (hr != ND_BUFFER_OVERFLOW)
			return hr;
		if (!listsize) {
			continue;
		}

		factory->addr_list = (SOCKET_ADDRESS_LIST*)malloc(listsize);
		if (!factory->addr_list)
			return ND_NO_MEMORY;

		hr = factory->provider->lpVtbl->QueryAddressList(factory->provider,
			factory->addr_list, &listsize);
		if (FAILED(hr))
			return hr;

		for (INT j = 0; j < factory->addr_list->iAddressCount; j++) {
			if (ofi_nd_is_valid_addr(factory->addr_list->Address[j].lpSockaddr))
				addr_count++;
		}
	}

	if (!addr_count)
		return E_NOINTERFACE;

	ofi_nd_infra.adapters.adapter = (struct adapter_t*)malloc(addr_count * sizeof(*ofi_nd_infra.adapters.adapter));
	if (!ofi_nd_infra.adapters.adapter)
		return ND_NO_MEMORY;

	/* put all available valid addresses into common array */
	for (size_t i = 0; i < ofi_nd_infra.class_factories.count; i++) {
		struct factory_t *factory = &ofi_nd_infra.class_factories.factory[i];
		for (INT j = 0; j < factory->addr_list->iAddressCount; j++) {
			if (ofi_nd_is_valid_addr(factory->addr_list->Address[j].lpSockaddr)) {
				struct adapter_t *adapter = &ofi_nd_infra.adapters.adapter[ofi_nd_infra.adapters.count];
				assert((int)sizeof(adapter->address) >= factory->addr_list->Address[j].iSockaddrLength);
				memcpy(&adapter->address,
				       factory->addr_list->Address[j].lpSockaddr,
				       factory->addr_list->Address[j].iSockaddrLength);
				adapter->factory = factory;
				ofi_nd_infra.adapters.count++;
			}
		}
	}

	if (!ofi_nd_infra.adapters.count)
		return E_NOINTERFACE;

	/* sort adapters by addresses to set IP4 addresses first. then remove
	   duplicates */
	qsort(ofi_nd_infra.adapters.adapter, ofi_nd_infra.adapters.count,
	      sizeof(struct adapter_t), ofi_nd_adapter_cmp);
	ofi_nd_infra.adapters.count = unique(ofi_nd_infra.adapters.adapter,
					     ofi_nd_infra.adapters.count,
					     sizeof(struct adapter_t), ofi_nd_adapter_cmp);

	for (size_t i = 0; i < ofi_nd_infra.adapters.count; i++) {
		struct adapter_t *adapter = &ofi_nd_infra.adapters.adapter[i];
		struct factory_t *factory = adapter->factory;
		wchar_t *saddr;
		DWORD addrlen = 0;
		UINT64 id;
		int res;

		assert(factory);
		assert(factory->provider);

		assert(adapter->address.addr.sa_family == AF_INET ||
		       adapter->address.addr.sa_family == AF_INET6);

		hr = factory->provider->lpVtbl->ResolveAddress(factory->provider,
			&adapter->address.addr,
			ofi_sizeofaddr(&adapter->address.addr), &id);

		if (FAILED(hr))
			return hr;

		hr = factory->provider->lpVtbl->OpenAdapter(factory->provider,
			&IID_IND2Adapter, id, (void**)&adapter->adapter);
		if (FAILED(hr))
			return hr;

		ULONG linfo = sizeof(adapter->info);
		adapter->info.InfoVersion = ND_VERSION_2;
		hr = adapter->adapter->lpVtbl->Query(adapter->adapter, &adapter->info, &linfo);
		if (FAILED(hr) && hr == ND_BUFFER_OVERFLOW) {
			ND2_ADAPTER_INFO *info = (ND2_ADAPTER_INFO*)malloc(linfo);
			if (!info)
				return ND_NO_MEMORY;
			info->InfoVersion = ND_VERSION_2;
			hr = adapter->adapter->lpVtbl->Query(adapter->adapter, info, &linfo);
			if (FAILED(hr))
				return hr;
			adapter->info = *info;
			free(info);
		} else if (FAILED(hr)) {
			return hr;
		}

		/* generate adapter's name */
		res = WSAAddressToStringW(&adapter->address.addr,
					     ofi_sizeofaddr(&adapter->address.addr),
					     NULL, NULL, &addrlen);
		if (res == SOCKET_ERROR && WSAGetLastError() == WSAEFAULT && addrlen) {
			saddr = (wchar_t*)malloc((addrlen + 1) * sizeof(*saddr));
			WSAAddressToStringW(&adapter->address.addr,
					    ofi_sizeofaddr(&adapter->address.addr),
					    NULL, saddr, &addrlen);
		}
		else {
			saddr = _wcsdup(L"unknown");
		}

		asprintf((char**)&adapter->name, "netdir-%S-%S-%p",
			 ofi_nd_filename(adapter->factory->module->path),
			 saddr, adapter);
		free(saddr);
	}

	return S_OK;
}

static HRESULT ofi_nd_init(ofi_nd_adapter_cb_t cb)
{
	DWORD proto_len = 0;
	HRESULT hr = ND_INTERNAL_ERROR;
	int i, protonum, err;
	size_t j, prov_count = 0;
	WSAPROTOCOL_INFOW *proto = 0;

	memset(&ofi_nd_infra, 0, sizeof(ofi_nd_infra));

	int ret = WSCEnumProtocols(NULL, NULL, &proto_len, &err);
	if (ret != SOCKET_ERROR || err != WSAENOBUFS) {
		hr = ND_NO_MEMORY;
		goto fn_exit;
	}

	proto = (WSAPROTOCOL_INFOW*)(malloc(proto_len));
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
		if (ofi_nd_is_valid_proto(&proto[i]))
			prov_count++;
	}

	if (!prov_count) {
		hr = E_NOINTERFACE;
		goto fn_protofail;
	}

	hr = ofi_nd_alloc_infra(prov_count);
	if (hr != S_OK)
		goto fn_protofail;

	for (i = 0; i < protonum; i++) {
		if (ofi_nd_is_valid_proto(&proto[i]))
			ofi_nd_create_factory(&proto[i]);
	}

	free(proto);

	/* ok, factories are created, now list all available addresses, try to
	   create adapters & collect adapter's info */
	hr = ofi_nd_create_adapter();
	if (FAILED(hr))
		return hr;

	/* free all interfaces. we don't need it right now */
	ofi_nd_release_infra();

	/* now call cb function to create info's */
	for (j = 0; j < ofi_nd_infra.adapters.count; j++)
		cb(&ofi_nd_infra.adapters.adapter[j].info,
		   ofi_nd_infra.adapters.adapter[j].name);

	return hr;
fn_protofail:
	free(proto);
fn_exit:
	return hr;
}

/* we don't need here exclusive execution because this function
 * is called from OFI init routine which is single thread */
HRESULT ofi_nd_startup(ofi_nd_adapter_cb_t cb)
{ 
	WSADATA data;
	HRESULT hr;
	int ret;

	assert(cb);

	if (ofi_nd_startup_done)
		return S_OK;

	ND_LOG_INFO(FI_LOG_CORE, "ofi_nd_startup: starting initialization\n");

	ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (ret)
		return HRESULT_FROM_WIN32(ret);

	ND_LOG_DEBUG(FI_LOG_CORE, "ofi_nd_startup: WSAStartup complete\n");

	hr = ofi_nd_init(cb);

	ofi_nd_startup_done = 1;

	return hr;
}

HRESULT ofi_nd_shutdown(void)
{
	if (!ofi_nd_startup_done)
		return S_OK;

	ND_LOG_INFO(FI_LOG_CORE, "ofi_nd_shutdown: shutdown WSA\n");

	ofi_nd_free_infra();
	ofi_nd_startup_done = 0;

	return HRESULT_FROM_WIN32(WSACleanup());
}

int ofi_nd_lookup_adapter(const char *name, IND2Adapter **adapter, struct sockaddr** addr)
{
	size_t i;

	assert(name);
	assert(adapter);

	if (!ofi_nd_startup_done)
		return -FI_EOPBADSTATE;

	for (i = 0; i < ofi_nd_infra.adapters.count; i++) {
		struct adapter_t *ada = &ofi_nd_infra.adapters.adapter[i];
		if (ada->name && !strcmp(ada->name, name)) {
			HRESULT hr;
			UINT64 adapter_id;
			IClassFactory* factory = NULL;
			IND2Provider *provider = NULL;

			/* ok, we found good adapter. try to initialize it */
			if (ada->adapter) {
				*adapter = ada->adapter;
				*addr = &ada->address.addr;
				ada->adapter->lpVtbl->AddRef(ada->adapter);
				return FI_SUCCESS;
			}

			assert(ada->factory);
			assert(ada->factory->module);
			assert(ada->factory->module->get_class_object);

			hr = ada->factory->module->get_class_object(
				&ada->factory->protocol.ProviderId,
				&IID_IClassFactory,
				(void**)&factory);
			if (FAILED(hr))
				return H2F(hr);
			assert(factory);

			hr = factory->lpVtbl->CreateInstance(factory, NULL,
							     &IID_IND2Provider,
							     (void**)&provider);
			factory->lpVtbl->Release(factory);
			if (FAILED(hr))
				return H2F(hr);
			assert(provider);

			hr = provider->lpVtbl->ResolveAddress(provider, &ada->address.addr,
							      ofi_sizeofaddr(&ada->address.addr),
							      &adapter_id);
			if (FAILED(hr)) {
				provider->lpVtbl->Release(provider);
				return H2F(hr);
			}

			hr = provider->lpVtbl->OpenAdapter(provider, &IID_IND2Adapter, adapter_id,
							   (void**)&ada->adapter);
			provider->lpVtbl->Release(provider);
			if (FAILED(hr))
				return H2F(hr);

			*adapter = ada->adapter;
			*addr = &ada->address.addr;
			ada->adapter->lpVtbl->AddRef(ada->adapter);

			return FI_SUCCESS;
		}
	}

	return -FI_EINVAL;
}

#endif /* _WIN32 */


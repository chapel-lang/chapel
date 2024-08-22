#include "rxm.h"

/**
 * @brief copy list of IOVs from/to the host to/from an hmem device
 *
 * @param buf       host buffer address
 * @param buf_size  size of the host buffer
 * @param hmem_iov  list of IOVs on the device
 * @param iov_count number of IOVs in the list
 * @param iov_offset offset in bytes in the IOVs struct (cummulative over IOVs)
 *
 * note: inspired by efa_copy_from/to_hmem_iov
 */
ssize_t rxm_copy_hmem_iov(void **desc, char *buf, size_t buf_size,
			  const struct iovec *hmem_iov, int iov_count,
			  size_t iov_offset, int dir)
{
	int i, ret = -1;
	ssize_t data_size = 0;

	for (i = 0; i < iov_count; i++) {
		char *hmem_buf;
		void *local_desc = (desc) ? desc[i] : NULL;
		size_t len = ofi_iov_bytes_to_copy(hmem_iov + i, &buf_size,
						   &iov_offset, &hmem_buf);
		if (!len)
			continue;

		ret = rxm_copy_hmem(local_desc, buf + data_size, hmem_buf, len,
				    dir);
		if (ret)
			return ret;

		data_size += len;
	}
	return data_size;
}

/**
 * @brief copy data from/to the host to/from an hmem device
 *
 * note: inspired by efa_copy_from/to_hmem
 */
ssize_t rxm_copy_hmem(void *desc, char *host_buf, void *dev_buf, size_t size,
		      int dir)
{
	int ret = FI_SUCCESS;
	uint64_t device = 0, flags = 0;
	enum fi_hmem_iface iface = FI_HMEM_SYSTEM;
	void *hmem_handle = NULL;

	if (desc) {
		iface = ((struct rxm_mr *) desc)->iface;
		device = ((struct rxm_mr *) desc)->device;
		flags = ((struct rxm_mr *) desc)->hmem_flags;
		hmem_handle = ((struct rxm_mr *) desc)->hmem_handle;
	}

	if (flags & OFI_HMEM_DATA_DEV_REG_HANDLE) {
		assert(hmem_handle);
		/* TODO: Fine tune the max data size to switch from dev_reg copy
		 * to ofi_hmem regular copy */
		if (dir == OFI_COPY_IOV_TO_BUF) {
			ofi_hmem_dev_reg_copy_from_hmem(
				iface, (uint64_t) hmem_handle, host_buf,
				dev_buf, size);
		} else {
			ofi_hmem_dev_reg_copy_to_hmem(iface,
						      (uint64_t) hmem_handle,
						      dev_buf, host_buf, size);
		}
		return FI_SUCCESS;
	}
	if (dir == OFI_COPY_IOV_TO_BUF) {
		ret = ofi_copy_from_hmem(iface, device, host_buf, dev_buf,
					 size);
	} else {
		ret = ofi_copy_to_hmem(iface, device, dev_buf, host_buf, size);
	}
	return ret;
};

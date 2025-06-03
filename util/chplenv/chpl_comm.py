#!/usr/bin/env python3
import sys
import os
import re

import overrides
from utils import memoize, check_valid_var, which


@memoize
def get():
    comm_val = overrides.get('CHPL_COMM')
    if not comm_val:
        network = get_network()
        if network == 'aries':
            comm_val = 'ugni'
        elif network == 'ibv':
            comm_val = 'gasnet'
        elif network == 'cxi':
            comm_val = 'ofi'
        else:
            # fall back to platform detection if we can't determine the network
            import chpl_platform
            platform_val = chpl_platform.get('target')
            # Use ugni on cray-xc series
            if platform_val == 'cray-xc':
                comm_val = 'ugni'
            # Use ofi on hpe-cray-ex
            elif platform_val == 'hpe-cray-ex':
                comm_val = 'ofi'
            # Use gasnet on cray-cs and hpe-apollo
            elif platform_val in ('cray-cs', 'hpe-apollo'):
                comm_val = 'gasnet'
            # default to ofi on cray-xd when we can't determine ib vs cxi
            elif platform_val == 'hpe-cray-xd':
                comm_val = 'ofi'
            else:
                comm_val = 'none'

    check_valid_var("CHPL_COMM", comm_val, ("none", "gasnet", "ofi", "ugni"))
    return comm_val


@memoize
def _is_aries():
    # Check for cray platform. It is a cray platform if there is a
    # cle-release/CLEinfo config file with a known network value in it.
    cle_info_file = os.path.abspath('/etc/opt/cray/release/cle-release') # CLE >= 6
    if not os.path.exists(cle_info_file):
        cle_info_file = os.path.abspath('/etc/opt/cray/release/CLEinfo') # CLE <= 5

    if os.path.exists(cle_info_file):
        with open(cle_info_file, 'r') as fp:
            cle_info = fp.read()
        net_pattern = re.compile('^NETWORK=(?P<net>[a-zA-Z]+)$', re.MULTILINE)
        net_match = net_pattern.search(cle_info)
        if net_match is not None and len(net_match.groups()) == 1:
            net = net_match.group('net')
            if net.lower() == 'ari':
                return True
    return False

@memoize
def _is_ibv():
    """
    Look for '/dev/infiniband/uverbs0' and '/sys/class/net/ib*'
    """
    return (which('ibstat') and
            os.path.exists('/dev/infiniband/uverbs0') and
            any(dev.startswith('ib') for dev in os.listdir('/sys/class/net/')))

@memoize
def _is_cxi():
    """
    Look for '/sys/class/net/hsn*', '/sys/class/cxi/', and '/dev/cxi*'
    """
    return (which('cxi_stat') and
            any(dev.startswith('hsn') for dev in os.listdir('/sys/class/net/')) and
            os.path.exists('/sys/class/cxi/') and
            any(dev.startswith('cxi') for dev in os.listdir('/dev/')))

@memoize
def get_network():
    """
    Guesses the network type of the current system.
    Returns 'cxi', 'ibv', 'aries', or 'unknown'
    """
    network = None
    if _is_aries():
        network = 'aries'
    elif _is_ibv():
        network = 'ibv'
    elif _is_cxi():
        network = 'cxi'
    else:
        network = 'unknown'
    return network

def _main():
    comm_val = get()
    sys.stdout.write("{0}\n".format(comm_val))


if __name__ == '__main__':
    _main()

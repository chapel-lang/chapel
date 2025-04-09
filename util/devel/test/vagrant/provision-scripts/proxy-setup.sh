#!/bin/sh
#
# Persists http_proxy and https_proxy variables in the VM environment if provided.

if [ -z "$http_proxy" ] || [ -z "$https_proxy" ]; then
    echo "http_proxy or https_proxy was unset, not setting these variables in the VM"
    exit 0
fi;

cat << EOF >> /etc/profile
export http_proxy=$http_proxy
export https_proxy=$https_proxy
export HTTP_PROXY=$http_proxy
export HTTPS_PROXY=$https_proxy
EOF

cat << EOF >> /etc/csh.cshrc
export http_proxy=$http_proxy
export https_proxy=$https_proxy
export HTTP_PROXY=$http_proxy
export HTTPS_PROXY=$https_proxy
EOF

# Hack to work on any sudoers file location
cat << EOF | EDITOR='tee -a' visudo
Defaults env_keep += "http_proxy"
Defaults env_keep += "https_proxy"
Defaults env_keep += "HTTP_PROXY"
Defaults env_keep += "HTTPS_PROXY"
EOF

cat << EOF >> /usr/local/etc/pkg.conf
pkg_env: {
    http_proxy: "$http_proxy",
    https_proxy: "$https_proxy",
}
EOF

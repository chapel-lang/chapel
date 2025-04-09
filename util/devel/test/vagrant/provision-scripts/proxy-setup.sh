#!/bin/sh
#
# Persists HTTP_PROXY and HTTPS_PROXY variables in the VM environment if provided.

if [ -z "$HTTP_PROXY" ] || [ -z "$HTTPS_PROXY" ]; then
    echo "HTTP_PROXY or HTTPS_PROXY was unset, not setting these variables in the VM"
    exit 0
fi;

cat << EOF >> /etc/profile
export http_proxy=$HTTP_PROXY
export https_proxy=$HTTPS_PROXY
export HTTP_PROXY=$HTTP_PROXY
export HTTPS_PROXY=$HTTPS_PROXY
EOF

cat << EOF >> /etc/csh.cshrc
export http_proxy=$HTTP_PROXY
export https_proxy=$HTTPS_PROXY
export HTTP_PROXY=$HTTP_PROXY
export HTTPS_PROXY=$HTTPS_PROXY
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
    http_proxy: "$HTTP_PROXY",
    https_proxy: "$HTTPS_PROXY",
}
EOF

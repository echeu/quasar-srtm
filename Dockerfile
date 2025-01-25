# Docker file for creating environment for building quasar Opc Ua Server
# ECC - move to Alma 9
FROM multiarch/qemu-user-static:x86_64-aarch64 as qemu
FROM cern/alma9-base@sha256:732d02636185f2720c606a199ffc858c28344865092f87f345f0bea13a072c82
COPY --from=qemu /usr/bin/qemu-aarch64-static /usr/bin

# packages
RUN dnf install -y gcc-c++
RUN dnf install -y openssh-clients 
RUN dnf install -y python3
RUN dnf install -y pip
RUN dnf install -y cmake3
RUN dnf install -y boost-devel
RUN dnf install -y boost-regex
RUN dnf install -y boost-thread
RUN dnf install -y boost-program-options
RUN dnf install -y openssl-devel
RUN dnf install -y libxml2-devel
RUN dnf install -y graphviz
RUN dnf install -y ninja-build
RUN dnf install -y wget
RUN dnf install -y git


# The following packages do not work with dnf and have to be loaded by hand
# get xerces
RUN wget https://www.rpmfind.net/linux/epel/9/Everything/aarch64/Packages/x/xerces-c-3.2.5-1.el9.aarch64.rpm
RUN rpm -Uvh xerces-c-3.2.5-1.el9.aarch64.rpm
RUN wget https://www.rpmfind.net/linux/epel/9/Everything/aarch64/Packages/x/xerces-c-devel-3.2.5-1.el9.aarch64.rpm
RUN rpm -Uvh xerces-c-devel-3.2.5-1.el9.aarch64.rpm

# get libcutl and xsd
RUN wget https://www.rpmfind.net/linux/epel/9/Everything/aarch64/Packages/l/libcutl-1.10.0-23.el9.aarch64.rpm
RUN rpm -Uvh libcutl-1.10.0-23.el9.aarch64.rpm
RUN wget https://www.rpmfind.net/linux/epel/9/Everything/aarch64/Packages/x/xsd-4.1.0-0.8.a11.el9.aarch64.rpm
RUN rpm -Uvh xsd-4.1.0-0.8.a11.el9.aarch64.rpm

# get astyle
RUN wget https://www.rpmfind.net/linux/epel/9/Everything/aarch64/Packages/a/astyle-3.1-15.el9.aarch64.rpm
RUN rpm -Uvh astyle-3.1-15.el9.aarch64.rpm


# python modules
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install colorama jinja2 enum34 
RUN python3 -m pip install lxml pygit2

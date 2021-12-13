# Docker file for creating environment for building quasar Opc Ua Server
# ECC - try using CS8 instead of C8
FROM multiarch/qemu-user-static:x86_64-aarch64 as qemu
FROM cern/cs8-base@sha256:15b6f0189c14f6185657904bdbc1f42db93274f2581fc509d3fbdae2e5980b50
COPY --from=qemu /usr/bin/qemu-aarch64-static /usr/bin

# packages
RUN dnf install -y gcc-c++
RUN dnf install -y openssh-clients 
RUN dnf install -y python3
RUN dnf install -y cmake3
RUN dnf install -y boost-devel
RUN dnf install -y boost-regex
RUN dnf install -y boost-thread
RUN dnf install -y boost-program-options
RUN dnf install -y openssl-devel
RUN dnf install -y libgit2
RUN dnf install -y libxml2-devel
RUN dnf install -y graphviz
RUN dnf install -y ninja-build
RUN dnf install -y wget
RUN dnf install -y git


# The following packages do not work with dnf and have to be loaded by hand
# get xerces
RUN wget https://download-ib01.fedoraproject.org/pub/epel/8/Everything/aarch64/Packages/x/xerces-c-3.2.2-3.el8.aarch64.rpm
RUN rpm -Uvh xerces-c-3.2.2-3.el8.aarch64.rpm
RUN wget https://download-ib01.fedoraproject.org/pub/epel/8/Everything/aarch64/Packages/x/xerces-c-devel-3.2.2-3.el8.aarch64.rpm
RUN rpm -Uvh xerces-c-devel-3.2.2-3.el8.aarch64.rpm

# get xsd
RUN wget https://download-ib01.fedoraproject.org/pub/epel/8/Everything/aarch64/Packages/l/libcutl-1.10.0-17.el8.aarch64.rpm
RUN rpm -Uvh libcutl-1.10.0-17.el8.aarch64.rpm
RUN wget https://download-ib01.fedoraproject.org/pub/epel/8/Everything/aarch64/Packages/x/xsd-4.1.0-0.2.a11.el8.aarch64.rpm
RUN rpm -Uvh xsd-4.1.0-0.2.a11.el8.aarch64.rpm

# get astyle
RUN wget https://download-ib01.fedoraproject.org/pub/epel/8/Everything/aarch64/Packages/a/astyle-3.1-8.el8.aarch64.rpm
RUN rpm -Uvh astyle-3.1-8.el8.aarch64.rpm

# python modules
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install colorama jinja2 enum34 
RUN python3 -m pip install lxml pygit2

# Docker

This provides information for setting up a docker environment for building the quasar-srtm UA server. Docker is used to allow one to compile and link the UA server for the target operating system (aarch64).

## Dockerfile

A Dockerfile is provided to allow one to easily install the requisite packages.
  - https://github.com/echeu/quasar-srtm/Dockerfile


## Build docker image

The following commands only have to be issued once to instantiate the docker image.
  - docker run --rm --privileged multiarch/qemu-user-static:register --reset
  - docker build --rm -t cern-arm64-cs8 - < Dockerfile

## Docker run

Do the following to enter the docker container.
  - docker run -it cern-arm64-cs8 /bin/bash

## Building the executable

  - git clone https://github.com/echeu/quasar-srtm.git
  - ./quasar.py enable_module open62541-compat v1.3.6
  - ./quasar.py set_build_config ./open62541_config.cmake
  - cd quasar-srtm
  - ./quasar.py build

FROM ubuntu:22.04
RUN apt-get update && apt-get install -y --no-install-recommends build-essential meson ninja-build libgtest-dev python3 gdb valgrind && rm -rf /var/lib/apt/lists/*
WORKDIR /work

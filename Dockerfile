# Set the Ubuntu image as the base image for building
FROM ubuntu:latest AS build

# Install build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libwayland-dev \
    xorg-dev

# Copy files into the container
COPY . /KumaGL

# Set the working directory and run CMake
WORKDIR /KumaGL
RUN mkdir build; \
    cd build; \
    cmake ..; \
    make install

# Set the Ubuntu image as the base image for running as well
FROM ubuntu:latest AS runtime

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libwayland-dev \
    xorg-dev

COPY --from=build /usr/local/lib /usr/local/lib
COPY --from=build /usr/local/include /usr/local/include
COPY --from=build /usr/local/bin /usr/local/bin

#WORKDIR /usr/local/bin/KumaGL/examples/cubes
RUN cd /usr/local/bin/KumaGL/examples/cubes; \
    ./cubes
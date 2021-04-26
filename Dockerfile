FROM ubuntu:20.04

RUN apt update
RUN apt install -y cmake g++ wget unzip
RUN apt install -y libopencv-imgcodecs-dev libopencv-features2d-dev


# Build OpenCV
# WORKDIR /tmp
# RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
# RUN unzip opencv.zip
# RUN wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/master.zip
# RUN unzip opencv_contrib.zip
ADD ./submodules/opencv /tmp/opencv
ADD ./submodules/opencv_contrib /tmp/opencv_contrib
RUN mkdir -p /tmp/opencv/build

WORKDIR /tmp/opencv/build
RUN cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
RUN make -j4
RUN make install


# Build Google Test
ADD ./submodules/googletest /tmp/googletest
RUN mkdir -p /tmp/googletest/build

WORKDIR /tmp/googletest/build
RUN cmake ..
RUN make -j8 && make install


# Add library path
RUN echo "/usr/local/lib" >> /etc/ld.so.conf.d/usr-local-lib.conf
RUN echo "/usr/local/include/gtest" >> /etc/ld.so.conf.d/usr-local-lib.conf
RUN ldconfig -v


WORKDIR /app




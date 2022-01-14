FROM ubuntu:20.04

ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

WORKDIR /z
RUN apt-get update -qq && apt-get upgrade -y -qq
RUN apt-get install build-essential mingw-w64 autoconf wine git curl cmake libgl1-mesa-dev libsndfile1-dev libsamplerate0-dev libfreetype-dev -y
COPY . .
RUN git clone https://github.com/DISTRHO/PawPaw.git
WORKDIR /z/PawPaw
RUN ./bootstrap-plugins.sh win64
RUN bash -c 'source local.env win64'
WORKDIR /z
RUN mv -f Resources/DPF-plugin.cmake dpf/cmake/DPF-plugin.cmake
RUN rm -rf build
RUN mkdir build
WORKDIR /z/build
RUN cmake -DWIN32 ../
RUN make

CMD []
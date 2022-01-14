################################################################################
FROM archlinux

#Build dependencies
RUN pacman -Syu --noconfirm

#Basics 
RUN pacman -Syu base-devel --noconfirm

RUN pacman -S cmake libglvnd libsndfile libsamplerate freetype2 --noconfirm

WORKDIR /z

COPY . .
RUN mv -f Resources/DPF-plugin.cmake dpf/cmake/DPF-plugin.cmake 
RUN rm -rf build
RUN mkdir build
WORKDIR /z/build
RUN cmake ../
RUN make

CMD []
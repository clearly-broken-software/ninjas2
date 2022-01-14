docker build -t ninjas2 -f ubuntu-linux.dockerfile .
docker create -ti --name ninjas2-ubuntu-linux ninjas2 bash
docker cp ninjas2-ubuntu-linux:/z/build/bin/ninjas2 .
docker rm ninjas2-ubuntu-linux
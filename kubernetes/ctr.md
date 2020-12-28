## ctr常用命名

  273  ctr t exec --help
  274  ctr t exec -t --exec-id 57528 gputest
  275  ctr t exec -t --exec-id 57528 gputest bash
  276  ctr t kill -s SIGKILL -a gputest
  277  ctr c rm gputest
  344  ctr c ls
  377  ctr i ls | grep cuba
  410  ctr c ls
  853  ctr run --rm -d --net-host --runtime=io.containerd.ecr.v2  --device /dev/vfio/20 docker.io/bevisy/centos:7-nvidia  gputest sleep 7d
  854  ctr c ls
  855  ctr t ls
  856  ctr c rm gputest
  857  ctr t ls
  858  ctr c ls
  859  ctr run --rm -d --net-host --runtime=io.containerd.ecr.v2  --device /dev/vfio/20 docker.io/bevisy/centos:7-nvidia  gputest sleep 7d
  861  ctr run --rm -d --net-host --runtime=io.containerd.ecr.v2  --device /dev/vfio/20 docker.io/bevisy/centos:7-nvidia  gputest sleep 7d
  862  ctr c rm gputest
  863  ctr run --rm -d --net-host --runtime=io.containerd.ecr.v2  --device /dev/vfio/20 docker.io/bevisy/centos:7-nvidia  gputest sleep 7d
  888  ctr run --rm -d --runtime=io.containerd.ecr.v2  --device /dev/vfio/20 docker.io/bevisy/centos:7-nvidia  gputest sleep 7d
  889  ctr c rm gputest
  890  ctr run --rm -d --runtime=io.containerd.ecr.v2  --device /dev/vfio/20 docker.io/bevisy/centos:7-nvidia  gputest sleep 7d


ctr run --rm docker.io/library/centos:8 jiangkai1 sleep 600s
ctr t exec -t --exec-id ${pid} gputest bash

docker run -it jiangkai/k8s-device-plugin:v1 sleep 120s
docker exec -it 1d2f62bda023 sh

docker load -i golang1.15.1.tar

echo "ls -l" > /usr/local/bin/ll;chmod 777 /usr/local/bin/ll




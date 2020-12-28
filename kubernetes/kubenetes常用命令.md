## kubenetes常用命令
ssh root@192.168.4.103
ssh -i ~/.ssh/id_rsa.roller 10.10.1.5

host: 127.0.0.1

#### 进入pod
kubectl get pod -n [namespace] | grep [pod name]    连接上node3之后，首先获取所有pod，namespace为openstack，进入pod
kubectl exec -it easystack-dm-rzbnp bash -n openstack    进入指定pod，拷贝代码
kubectl cp [soure file] pod:[dst file path] -n [namespace]    拷贝本地代码到pod以方便进行调试，代码拷贝后需要重新编译并重启

kubectl exec -it bash -n openstack
kubectl logs -n openstack
kubectl delete pod -n openstack

####查看pod
kubectl get pod -n openstack -o wide --show-labels | grep easystack-dm
kubectl get pod -n openstack -o wide --show-labels | grep -E "easystack-dm|easystack-vmm"
####查看pod日志
kubectl logs -n openstack easystack-dm-fzbj6
####查看usb设备
kubectl get nodeusb -n openstack -o wide --show-labels
####查看单个usb设备，以yaml格式查看详细信息
kubectl get nodeusb -n openstack node-3-1-10 -o yaml
####查看全量crd信息
kubectl get crd -n openstack --show-labels
####查看指定crd信息
kubectl get nodeusbs.nodeinfo.com -n openstack -o yaml
####查看node信息
kubectl get node -n openstack --show-labels -o wide
####修改容器daemonset
kubectl edit daemonset -n openstack
####查看容器异常日志原因
kubectl describe pod easystack-vmm-pr5dv -n openstack

##################################################################################################################
kubectl delete deployment easystack-dm -n openstack
kubectl apply -f dm-deployment.yml
kubectl get pod -n openstack | grep dm
##################################################################################################################

dmesg | grep -A 10 1111300000040750
lsblk | grep "sdf"


# 删除Pod
kubectl delete pod easystack-dm-rzbnp -n openstack
kubectl get pods -n openstack | grep -E "easystack-dm|easystack-vmm" | awk '{print $1}' | xargs kubectl delete pod -n openstack
--force --grace-period=0
# 删除Deploy
kubectl delete deploy {deploy_name}
# 查看Pod列表
kubectl get pods -n kube-system
# 查看某个Pod信息
$ kubectl get pod etcd-kube-master3 -n kube-system
NAME                READY   STATUS    RESTARTS   AGE
etcd-kube-master3   1/1     Running   0          24d

#查看Pod详细信息
$ kubectl get pod etcd-kube-master3 -n kube-system -o wide
NAME                READY   STATUS    RESTARTS   AGE   IP             NODE           NOMINATED NODE   READINESS GATES
etcd-kube-master3   1/1     Running   0          24d   10.120.200.3   kube-master3   <none>           <none>

#查看Pod详细信息
kubectl describe pod {pod-name} -n kube-system

#查询pod
kubectl get pod --selector name=redis -n kube-system

#查看日志
kubectl logs {pod-name} -n kube-system

#查看服务
kubectl get svc -n kube-system

#查看Node列表
kubectl get nodes

#查看版本
kubectl version

#查看Deployment列表
kubectl get deployments -n kube-system

#添加label
kubectl label pod {pod-name} app=v1

#根据label查询
kubectl get pods -l app=v1 -n kube-system


2、pod相关命令
#进入某个pod进行交互

kubectl exec --it ${pod_name} [-c ${container_name}] -- /bin/bash

3、node相关命令
#给node添加标签

kubectl label nodes kube-node node=kube-node

#给node删除标签

kubectl label nodes kube-node node-

#根据标签查询节点

kubectl get node -a -l "node=kube-node"

## ConfigMap

### 概览
ConfigMap 在运行时会将配置文件、命令行参数、环境变量、端口号以及其他配置工件绑定到 Pod 的容器和系统组件。借助 ConfigMap，您可以将配置与 Pod 和组件分开，这有助于保持工作负载的可移植性，使其配置更易于更改和管理，并防止将配置数据硬编码到 Pod 规范。

ConfigMap 可用于存储和共享非敏感、未加密的配置信息。要在集群中使用敏感信息，您必须使用 Secret。

### 创建
使用以下命令创建 ConfigMap：
```
kubectl create configmap name data
```
其中：

 * name 是 ConfigMap 对象的名称。
 * data 可以是：
    - 包含一个或多个配置文件的目录的路径，使用 --from-file 标志指示。
    - 键值对，每个键值对都使用 --from-literal 标志指定。
如需详细了解 kubectl create，请参阅参考文档。

您还可以通过在 YAML 清单文件中定义 ConfigMap 对象并使用 kubectl create -f filename 部署对象来创建 ConfigMap。

#### 基于文件创建
如需基于一个或多个文件创建 ConfigMap，请使用 --from-file。您可以指定任何明文格式（例如 .properties、.txt 或 .env）的文件，只要这些文件包含键值对即可。

在 Git 中存储 ConfigMap 文件是最佳做法。这样，您就可以利用持续集成和交付 (CI/CD) 流水线。如需详细了解如何使用此工作流，请参阅使用 Cloud Build 实现 GitOps 形式的持续交付。

您可以传入单个文件或多个文件：
```
kubectl create configmap name --from-file path/to/file.properties --from-file path/to/file2.properties
```
还可以传入包含多个文件的目录：
```
kubectl create configmap name --from-file path/to/directory
```
在基于文件创建 ConfigMap 时，键默认为文件的基名，值默认为文件的内容。您还可以指定备用键。

对于基于目录的 ConfigMap，基名为目录中的有效键的每个文件都打包到 ConfigMap。kubectl 会忽略非常规文件，例如符号链接、设备和管道。子目录也会被忽略；kubectl create configmap 不会递归到子目录中。

例如，以下命令从目录 game-configs（该目录中包含两个 properties 文件：game.properties 和 ui.properties）创建一个名为 game-data 的 ConfigMap：

kubectl create configmap game-data --from-file game-configs/

运行 kubectl get configmap game-data -o yaml 会返回以下输出：
```
apiVersion: v1
data:
  game.properties: |
    enemies=aliens
    lives=3
    ...
  ui.properties: |
    color.good=purple
    color.bad=yellow
    allow.textmode=true
    ...
kind: ConfigMap
metadata:
  creationTimestamp: ...
  name: game-data
  namespace: default
  resourceVersion: ...
  selfLink: /api/v1/namespaces/default/configmaps/game-data
  uid: ...
```

#### 基于字面量值创建

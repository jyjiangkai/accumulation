字符串（string）作为一种不可变类型，在与字节数组（slice, [ ]byte）转换时需付出 “沉重” 代价，根本原因是对底层字节数组的复制。这种代价会在以万为单位的高并发压力下迅速放大，所以对它的优化常变成 “必须” 行为。
首先，我们看一下 string 和 []byte 的数据结构
```
struct string {
  uint8 *str;
  int len;
}

struct []uint8 {
  uint8 *array;
  int len;
  int cap;
}
```
推荐使用如下方式进行转换：
```
func str2bytes(s string) []byte {
  x := (*[2]uintptr)(unsafe.Pointer(&s))
  h := [3]uintptr{x[0], x[1], x[1]}
  return *(*[]byte)(unsafe.Pointer(&h))
}

func bytes2str(b []byte) string {
  return *(*string)(unsafe.Pointer(&b))
}
```
通过golang自带的性能测试框架测试发现性能优化明显，且没有垃圾回收。

内容来源于：`https://www.jianshu.com/p/0b8387057050`

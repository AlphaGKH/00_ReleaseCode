# 优先队列
## 1. 优先队列和普通队列的区别
- 普通的队列是一种**先进先出**的数据结构,元素在队尾追加,从队头删除.  
- 优先队列中,队列中的每个元素被赋予优先级.优先队列具有**最高优先级先出**的特征.
## 2. 优先队列的特点
优先队列具有队列的所有特征以及基本操作,只是在此基础上增加了内部的一个排序,它本质上是通过一个堆实现的.
> - top()   : 访问队头元素
> - empty() : 队列是否为空
> - size() :  返回队列内元素个数
> - push() :  插入元素到队尾 (优先队列还会进行排序)
> - emplace() : 原地构造一个元素并插入队列
> - pop() : 弹出队头元素
> - swap() : 交换内容

优先队列的原型为:
```C++
priority_queue<Type, Container, Functional>
```
`Type` 就是数据类型，`Container` 就是容器类型（`Container`必须是用数组实现的容器，比如`vector`,`deque`等等，但不能用 `list`。STL里面默认用的是`vector`），`Functional` 就是比较的方式

当需要用自定义的数据类型时才需要传入这三个参数，使用基本数据类型时，只需要传入数据类型，默认是大顶堆。

```C++
#include <iostream>
#include <queue>

int main(int, char**) {
    // 基本数据类型使用优先队列时,只需要类型Type这个参数,并且默认是大顶堆(降序)
    std::priority_queue<int> pq;
    for(int i = 0; i < 10; i++){
        pq.push(i);
    }

    while (!pq.empty())
    {
        auto a = pq.top();
        std::cout << a << " ";
        pq.pop();
    }
    std::cout << std::endl;

    return 0;
}
```
输出为:
```
9 8 7 6 5 4 3 2 1 0 
```

## 3. 
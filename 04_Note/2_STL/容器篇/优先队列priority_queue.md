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

**当需要用自定义的数据类型时才需要传入这三个参数，使用基本数据类型时，只需要传入数据类型，默认是大顶堆。**
### 2.1 优先队列使用基本类型
使用基本类型时,三个参数中只需要输入Type,这时默认是按照降序排列

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
如果想要升序排列那么需要三个参数都输入:
```C++
#include <iostream>
#include <queue>

int main(int, char**) {
    // 三个参数都输入, greater是升序排列
    std::priority_queue<int, std::vector<int>, std::greater<int> > pq;
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
输出为
```
0 1 2 3 4 5 6 7 8 9 
```
### 2.2 优先队列使用`pair`类型
`pair`类型做比较时,先比较`pair`中的第一个元素,如果第一个元素相等,那么再比较第二个元素.
```C++
#include <iostream>
#include <queue>

int main(int, char**) {
    std::priority_queue<std::pair<int, int> > pq;

    pq.emplace(1,2);
    pq.emplace(1,3);
    pq.emplace(2,5);

    while (!pq.empty())
    {
        auto a = pq.top();
        std::cout << "(" << a.first << "," << a.second << ")" << " ";
        pq.pop();
    }
    std::cout << std::endl;

    return 0;
}
```
输出为:
```
(2,5) (1,3) (1,2) 
```
### 2.3 优先队列使用自定义类型
使用自定义类型的数据作为优先队列的元素时,最核心的一点就是要定义自定义数据类型的比较方式,而定义比较方式又有**运算符重载**和**仿函数**两种方式.
- 采用**运算符重载**的方式
```C++
#include <iostream>
#include <queue>

class DataType {
public:
  DataType(bool b, int a, double c): b_(b), a_(a), c_(c){};
  ~DataType(){};

public:
  bool b() const { return b_; }
  int a() const { return a_; }
  double c() const { return c_; }
  // 重载<运算符,使元素降序排列,比较的规则时先比较第一个元素,第一个元素相等,比较第二个,
  // 第二个也相等就比较第三个.bool元素的比较规则为fasle小true大
  bool operator<(const DataType &d) const {
    if (b_ == d.b()) {
      if (a_ < d.a()) {
        return true;
      } else if (a_ > d.a()) {
        return false;
      } else {
        if (c_ <= d.c()) {
          return true;
        } else {
          return false;
        }
      }
    } else {
      if (b_) {
        return false;
      } else {
        return true;
      }
    }
  }

private:
  bool b_ = false;
  int a_ = 0;
  double c_ = 0.0;
};

int main(int, char **) {
  std::priority_queue<DataType> pq;

  pq.emplace(true, 1, 2);
  pq.emplace(false, 1, 3);
  pq.emplace(false, 2, 5);

  while (!pq.empty()) {
    auto a = pq.top();
    std::cout << "(" << a.b() << "," << a.a() << "," << a.c() << ")"
              << " ";
    pq.pop();
  }
  std::cout << std::endl;

  return 0;
}

```
输出为
```
(1,1,2) (0,2,5) (0,1,3) 
```
- 采用**仿函数**的方式
需要注意的是,仿函数并不是函数,它仍然是一个类,只是重载了`()`运算符,从而可以像函数一样调用
```C++
#include <iostream>
#include <queue>

class DataType {
public:
  DataType(bool b, int a, double c) : b_(b), a_(a), c_(c){};
  ~DataType(){};

public:
  bool b() const { return b_; }
  int a() const { return a_; }
  double c() const { return c_; }

private:
  bool b_ = false;
  int a_ = 0;
  double c_ = 0.0;
};

class CompareFunc {
public:
  bool operator()(const DataType &one, const DataType &other) {
    if (one.b() == other.b()) {
      if (one.a() < other.a()) {
        return true;
      } else if (one.a() > other.a()) {
        return false;
      } else {
        if (one.c() <= other.c()) {
          return true;
        } else {
          return false;
        }
      }
    } else {
      if (one.b()) {
        return false;
      } else {
        return true;
      }
    }
  }
};

int main(int, char **) {
  std::priority_queue<DataType, std::vector<DataType>, CompareFunc> pq;

  pq.emplace(true, 1, 2);
  pq.emplace(false, 1, 3);
  pq.emplace(false, 2, 5);

  while (!pq.empty()) {
    auto a = pq.top();
    std::cout << "(" << a.b() << "," << a.a() << "," << a.c() << ")"
              << " ";
    pq.pop();
  }
  std::cout << std::endl;

  return 0;
}
```
输出为:
```
(1,1,2) (0,2,5) (0,1,3) 
```

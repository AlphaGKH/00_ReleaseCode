# 单例模式
## 1. **单例模式的定义**
确保一个类在全局只有一个实例,且该类提供一个全局访问点来访问这个全局唯一的实例.
## 2. **单例模式的特点**
- **这个类在全局中只有一个唯一的实例**
- **这个类必须自己创建这个实例**
- **这个类提供一个全局接口来访问这个唯一的实例**



``` C++
#include <iostream>

class TaskManager {
public:
  // 全局接口用于访问唯一的实例
  static TaskManager *Instance() {
    if (!tm_) {
      tm_ = new (std::nothrow) TaskManager();
      std::cout << "Create Object" << std::endl;
    } else {
      std::cout << "The Object has existed!" << std::endl;
    }

    return tm_;
  }

public:
  void PrintNumber() { std::cout << "Number is " << number_ << std::endl; }

  void SetNumber(const double &number) {
    number_ = number;
    std::cout << "Set number: " << number << std::endl;
  }

private:
  TaskManager(); // 私有的构造函数,保证只能在类内部实例化对象  

  // 删除下面连个函数保证了对象不能拷贝也不能赋值给其他变量
  TaskManager(const TaskManager &) = delete; // 删除拷贝构造函数
  TaskManager &operator=(const TaskManager &) = delete; // 删除赋值构造函数

private:
  double number_ = 0;

  static TaskManager *tm_; // 全局唯一的实例
  // 这里为什么要使用static呢?原因是,Instance()函数是个static函数,只能调用静态成员变量
  // 需要注意的类的静态数据成员在类内只是声明,并未分配内存,需要在类外进行定义和初始化
};

// 将构造函数在类外实现的原因是,一般情况下,构造一个对象时需要进行很多的操作,不适宜在类内实现这个函数
TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
}

TaskManager *TaskManager::tm_ = nullptr; // 这里初始化类的静态数据成员

int main() {
  TaskManager::Instance()->PrintNumber();

  TaskManager::Instance()->SetNumber(10.5);

  TaskManager::Instance()->PrintNumber();
}

```
上述代码的输出为:
```
Constructor Function!
Create Object
Number is 0
The Object has existed!
Set number: 10.5
The Object has existed!
Number is 10.5
```
这里可以看出第一次调用`TaskManager::Instance()->PrintNumber()`时, 调用了构造函数创建了一个`TaskManager`的对象;  
第二次调用`TaskManager::Instance()->SetNumber(10.5)`时, 没有调用构造函数从新创建对象,而是将第一次创建的对象返回.

## 3. **懒汉模式和饿汉模式**
###  3.1 **懒汉模式**
懒汉模式, 顾名思义就是不到万不得已不会创建实例对象.换句话书,在第一次调用该类的实例的时候,该类才会去实例化一个对象作为单例的全局唯一对象.  
比如上面的代码,其实就是懒汉模式,只有在第一次调用`TaskManager::Instance()`的时候,才会实例化一个`TaskManager`类型的实例`tm_`.  
但是也要注意到,上面懒汉模式的代码是非线程安全的.比如:
```C++
#include <iostream>
#include <thread>

class TaskManager {
    ...
    ...
    ...
};

TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

TaskManager *TaskManager::tm_ = nullptr;

void ThreadFunc(const double &number) {
  TaskManager::Instance()->SetNumber(number);

  std::this_thread::sleep_for(std::chrono::duration<int>(1));

  return;
}

int main() {
  auto t1 = std::thread(&ThreadFunc, 10.5);
  auto t2 = std::thread(&ThreadFunc, 20.6);

  t1.join();
  t2.join();
  return 0;
}

```

输出为:
```
Constructor Function!
Constructor Function!
Create Object
Set number: Create Object
Set number: 10.5
20.6
```


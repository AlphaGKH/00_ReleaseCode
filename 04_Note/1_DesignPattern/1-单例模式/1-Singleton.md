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
public:
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
  TaskManager();
  TaskManager(const TaskManager &) = delete; 
  TaskManager &operator=(const TaskManager &) = delete;

private:
  double number_ = 0;

  static TaskManager *tm_;
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
从上面的输出可以看出,调用了两次构造函数,构建了两个实例对象.为什么会这样呢,这是因为线程`t1`调用`TaskManager::Instance()`时,由于是首次调用,所以`tm_=nullptr`,因此会调用构造函数实例化一个对象,但是就在构造函数`TaskManager::TaskManager()`构建的过程中,`t2`线程也启动了,这个时候`t2`线程调用`TaskManager::Instance()`时,`tm_`还没有构造完成,因此仍然`tm_=nullptr`,所以在`t2`线程中也会调用构造函数`TaskManager::TaskManager()`这就导致了实例化了两个对象.  
如何才能避免上上述情况呢?需要构建线程安全的单例类.

### 3.2 **线程安全的懒汉模式**
#### 1)  通过锁实现
线程安全第一个想到的就是加锁.
```C++
#include <iostream>
#include <mutex>
#include <thread>

class TaskManager {
public:
  static TaskManager *Instance() {
    /* 这里为什么有两次判非空操作?原因是:如果tm_已经实例化,那么直接就返回tm_就可以了,
     * 根本就不需要锁,只有在tm_未实例化的时候,才需要锁.当前线程如果能够获取到该锁,那么
     * 继续往下执行,如果不能获取到该锁,那么当前线程就阻塞在此处等待锁.当前线程获取到锁之
     * 后,再次判断tm_是否已经实例化,如果已经实例化,那么就直接返回tm_,如果没有实例化,
     * 那么当前线程就去实例化tm_.为什么当前线程获取到锁之后,需要再次判断tm_是否实例化
     * 呢?这是因为在当前线程因为锁而阻塞的时候,其他线程可能已经实例化了tm_
     */
    if (!tm_) {
      std::lock_guard<std::mutex> lock(lock_);
      if (!tm_) {
        tm_ = new (std::nothrow) TaskManager();
        std::cout << "Create Object" << std::endl;
      }

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
  TaskManager();
  TaskManager(const TaskManager &) = delete;
  TaskManager &operator=(const TaskManager &) = delete;

private:
  double number_ = 0;

  static TaskManager *tm_;

  static std::mutex lock_; // 声明锁
};

TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

TaskManager *TaskManager::tm_ = nullptr;
std::mutex TaskManager::lock_; // 定义锁

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
Create Object
Set number: 10.5
Set number: 20.6
```
上面的代码,首先启动线程`t1`,线程`t1`获取锁,并调用`TaskManager::TaskManager()`去实例化`tm_`;   
恰在此时线程`t2`启动,由于线程`t1`中调用的`TaskManager::TaskManager()`还没有完成对`tm_`的实例化,所以这个时候在线程`t2`调用`TaskManager::Instance()`时,仍然有`tm_ == nullptr`,那么线程`t2`中就去竞争锁,但是这个时候锁已经被线程`t1`占据,所以线程`t2`阻塞,等到线程`t2`获取到锁的时候,其实线程`t1`已经执行完毕了构造函数`TaskManager::TaskManager()`,完成了对 `tm_`的实例化,所以这个时候获取到锁的线程`t2`还得再次判断`tm_`是否为空.

这就是著名的双检查锁方式,但是这种方式并不是完美的解决线程安全的方式,主要是以下原因:
- 由于编译器的优化,可能导致内存读写存在reorder,进而导致双检查锁的方式失效;  

> 这种情况只存在与弱顺序内存模型的平台如powerPC,arm等,x86平台是强顺序内存模型平台,是不会出现上述问题的.  
怎么理解内存reorder这个问题呢?在上面的例子中,线程`t1`在获取锁之后,向下执行并分配内存空间用于实例化`tm_`,但是由于编译器的优化作用可能导致首先给`tm_`分配一块内存空间,然后在在此空间上对`tm_`进行实例化,但是在给`tm_`分配了内存空间之后,到完成`tm_`的实例化之前这段时间内,线程`t2`可能通过`TaskManager::Instance()`访问`tm_`这个指针,这个时候的`tm_`的确是指向一块内存的,但是指向的对象为完成实例化,从而导致因调用未实例化的对象而产生错误.

#### 2)  通过原子类型和内存栅栏进一步优化
这里的优化主要针对弱顺序内存模型平台.
```C++
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>

class TaskManager {
public:
  static TaskManager *Instance() {
    TaskManager* temp = tm_.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (!temp) {
      std::lock_guard<std::mutex> lock(lock_);
      temp = tm_.load(std::memory_order_relaxed);
      if (!temp) {
        temp = new (std::nothrow) TaskManager();
        std::atomic_thread_fence(std::memory_order_release);
        tm_.store(temp, std::memory_order_relaxed);

        std::cout << "Create Object" << std::endl;
      }

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
  TaskManager();
  TaskManager(const TaskManager &) = delete;
  TaskManager &operator=(const TaskManager &) = delete;

private:
  double number_ = 0;

  static std::atomic<TaskManager*> tm_;

  static std::mutex lock_; // 声明锁
};

TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

std::atomic<TaskManager*> TaskManager::tm_;

std::mutex TaskManager::lock_; // 定义锁

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
牵扯到C++的内存模型和内存栅栏相关知识,后续再分析

#### 3)  通过call_once实现线程安全的懒汉模式
```C++
#include <iostream>
#include <mutex>
#include <thread>

class TaskManager {
public:
  static TaskManager *Instance() {
    if (!tm_) {
      std::call_once(flag_, [&]() {
        tm_ = new (std::nothrow) TaskManager();
        std::cout << "Create Object" << std::endl;
      });

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
  TaskManager();
  TaskManager(const TaskManager &) = delete;
  TaskManager &operator=(const TaskManager &) = delete;

private:
  double number_ = 0;

  static TaskManager *tm_;

  static std::once_flag flag_;
};

TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

TaskManager *TaskManager::tm_ = nullptr;

std::once_flag TaskManager::flag_;

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
`call_once`的原型为:  
```C++
template<typename _Fn， typename... _Args>
inline void call_once(once_flag& _Flag, _Fn&& _Fx, _Args&&... _Ax);
```
其作用就是,针对于绑定到`call_once`中的`once_flag`对象_Flag,如果之前未有过有效的std::call_once调用,那么其参数_Fx（或其副本）会被调用;  
如果有过有效的std::call_once，那么在之后调用std::call_once时,就会直接返回而不执行_Fx.

###  3.3 **饿汉模式**
饿汉模式,顾名思义,就是不管用户是否会使用到单例对象,该类都会实例化一个对象.很显然这种模式的单例是线程安全的.
```C++
#include <iostream>
#include <mutex>
#include <thread>

class TaskManager {
public:
  static TaskManager *Instance() { return tm_; }

public:
  void PrintNumber() { std::cout << "Number is " << number_ << std::endl; }

  void SetNumber(const double &number) {
    number_ = number;
    std::cout << "Set number: " << number << std::endl;
  }

private:
  TaskManager();
  TaskManager(const TaskManager &) = delete;
  TaskManager &operator=(const TaskManager &) = delete;

private:
  double number_ = 0;

  static TaskManager *tm_;

  static std::once_flag flag_;
};

TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

// 不管是否用到单例的对象,这里都给它实例化一个
TaskManager *TaskManager::tm_ = new (std::nothrow) TaskManager();

std::once_flag TaskManager::flag_;

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
其输出如下:
```
Constructor Function!
Set number: Set number: 10.520.6
```
###  3.4 **懒汉模式和饿汉模式各自的特点**
懒汉的优点时,不调用就不会产生单例的对象,节省内存空间,属于以时间换空间;
饿汉不管是否调用都会产生一个实例对象,调用的时候拿来就用且线程安全,属于以空间换时间.
饿汉模式更加适合于访问单例对象的线程非常多的情况

## 4. **Apollo中的单例模式**
apollo项目中的单例模式是使用`call_once`实现的线程安全懒汉式单例.
```C++
#include <iostream>
#include <mutex>
#include <thread>

#define DECLARE_SINGLETON(classname)                                      \
 public:                                                                  \
  static classname *Instance() {                                          \
    static classname *instance = nullptr;                                 \
    if (!instance) {                                                      \
      static std::once_flag flag;                                         \
      std::call_once(flag,                                                \
                     [&] { instance = new (std::nothrow) classname(); }); \
    }                                                                     \
    return instance;                                                      \
  }                                                                       \
 private:                                                                 \
  classname();                                                            \
  classname(const classname &) = delete;                                  \
  classname &operator=(const classname &) = delete;



class TaskManager {
public:
  ~TaskManager();

public:
  void PrintNumber() { std::cout << "Number is " << number_ << std::endl; }

  void SetNumber(const double &number) {
    number_ = number;
    std::cout << "Set number: " << number << std::endl;
  }

private:
  double number_ = 0;
  DECLARE_SINGLETON(TaskManager)
};

TaskManager::TaskManager() {
  std::cout << "Constructor Function!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

TaskManager::~TaskManager() {}

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
Set number: Set number: 10.5
20.6
```
apollo中的实现方式有三个特点:
- 使用宏来定义一部分代码,减少的重复代码;
- 增加了一个public的析构函数;
- 未在采用类内声明类外定义的静态数据成员来代表单例的唯一实例对象,而是在`Instance()`函数中使用了静态局部对象的方式.这种方式的优点是获得了更好的兼容性,因为在C++11之前,类的静态成员变量在类外定义(或叫做初始化)时不是线程安全的,采用静态局部对象的方式就避免了这个问题.





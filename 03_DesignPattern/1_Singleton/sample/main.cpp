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
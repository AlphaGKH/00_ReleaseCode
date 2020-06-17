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

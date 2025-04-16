#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <atomic>
#include <chrono>

using namespace std;

atomic<bool> running(true); // Флаг завершения

// Простая структура запроса
struct Request {
  int reqClass;
  int reqType;
  int duration;

  Request(int cls, int type, int dur) : reqClass(cls), reqType(type), duration(dur) {}
};

// Простой вывод
void output(bool isDevice, bool start, int id, int reqClass, int reqType, int duration) {
  if (isDevice) {
    if (start)
      cout << "[Device " << id << "] Started request {Class: " << reqClass << ", Type: " << reqType << ", Duration: " << duration << "} \n";
    else
      cout << "[Device " << id << "] Finished request {Class: " << reqClass << ", Type: " << reqType << ", Duration: " << duration << "} \n";
  } else {
    cout << "[Generator] Created request {Class: " << reqClass << ", Type: " << reqType << ", Duration: " << duration << "} \n";
  }
}

class RequestQueue {
private:
  vector<Request> queue;
  mutex mtx;
  int maxSize;
  condition_variable not_full;
  condition_variable not_empty;

public:
  RequestQueue(int capacity) : maxSize(capacity) {}

  void push(const Request& req) {
    unique_lock<mutex> lock(mtx);
    not_full.wait(lock, [this]() { return queue.size() < maxSize || !running; });
    if (!running) return;

    auto index = lower_bound(queue.begin(), queue.end(), req,
      [](const Request& a, const Request& b) {
        return a.reqType > b.reqType;
      });
    queue.insert(index, req);

    not_empty.notify_one();
  }

  Request pop(int reqClass) {
    unique_lock<mutex> lock(mtx);
    not_empty.wait(lock, [this, reqClass]() {
      if (!running) return true;
      for (auto& r : queue) {
        if (r.reqClass == reqClass) return true;
      }
      return false;
    });

    if (!running && queue.empty()) return Request(-1, -1, 0);

    auto it = find_if(queue.begin(), queue.end(), [&](const Request& req) {
      return req.reqClass == reqClass;
    });

    if (it == queue.end()) return Request(-1, -1, 0); // Ничего не найдено

    Request req = *it;
    queue.erase(it);

    not_full.notify_one();
    return req;
  }

  void shutdown() {
    not_empty.notify_all();
    not_full.notify_all();
  }
};

class Generator {
private:
  RequestQueue& queue;
  int numberOfGroups;
public:
  Generator(RequestQueue& _queue, int _numberOfGroups) : queue(_queue), numberOfGroups(_numberOfGroups) {
    srand(time(0));
  }

  void generateRequest() {
    while (running) {
      int randomClass = rand() % numberOfGroups + 1;
      int randomType = rand() % 3 + 1;
      int randomDuration = rand() % 11 + 5;
      int randomTime = rand() % 5 + 1;

      Request newRequest(randomClass, randomType, randomDuration);
      queue.push(newRequest);

      output(false, false, -1, randomClass, randomType, randomDuration);

      this_thread::sleep_for(chrono::seconds(randomTime));
    }
  }
};

class Device {
private:
  RequestQueue& queue;
  int reqClass;
  int id;
public:
  Device(int _reqClass, RequestQueue& _queue, int _id) : reqClass(_reqClass), queue(_queue), id(_id) {}

  void work() {
    while (running) {
      Request req = queue.pop(reqClass);
      if (!running || req.reqClass == -1) break;

      output(true, true, id, req.reqClass, req.reqType, req.duration);
      this_thread::sleep_for(chrono::seconds(req.duration));
      output(true, false, id, req.reqClass, req.reqType, req.duration);
    }
  }
};

// Обработчик сигнала завершения
void signalHandler(int signum) {
  running = false;
  cout << "\n[!] Получен сигнал завершения (Ctrl + C). Завершаем...\n";
}

int main() {
  signal(SIGINT, signalHandler); // Установка обработчика Ctrl+C
  setlocale(0, "RU");

  int storageСapacity, numberOfGroups, numberOfDevices;
  cout << "Введите ёмкость накопителя: ";
  cin >> storageСapacity;
  cout << endl << "Введите количество групп приборов: ";
  cin >> numberOfGroups;
  cout << endl << "Введите количество приборов в каждой группе: ";
  cin >> numberOfDevices;
  cout << endl;

  RequestQueue queue(storageСapacity);
  Generator generator(queue, numberOfGroups);

  thread generating([&]() {
    generator.generateRequest();
  });

  vector<Device> devices;
  int id = 0;

  for (int i = 0; i < numberOfGroups; i++) {
    for (int j = 0; j < numberOfDevices; j++) {
      devices.emplace_back(i + 1, queue, id++);
    }
  }

  vector<thread> deviceThreads;
  for (auto& device : devices) {
    deviceThreads.emplace_back([&device]() {
      device.work();
    });
  }

  generating.join();
  queue.shutdown(); // Пробуждаем потоки, если они "спят"

  for (auto& t : deviceThreads) {
    t.join();
  }

  cout << "[*] Программа завершена корректно." << endl;
  return 0;
}

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <future>

class Algorithm {
public:
    virtual ~Algorithm() {}
    virtual int search(unsigned char *x, int m, unsigned char *y, int n) = 0;
    virtual const char* name() const = 0;
    const int timeout_ms = 1e4;

    long long search_with_time(unsigned char *x, int m, unsigned char *y, int n) {
        auto start_time = std::chrono::high_resolution_clock::now();

        // Запуск поиска в отдельном потоке
//        std::packaged_task<int()> task([this, x, m, y, n]() {
//            return this->search(x, m, y, n);
//        });
//        std::future<int> result = task.get_future();
//        std::thread(std::move(task)).detach();
//
//        // Ожидание результата с таймаутом
//        if (result.wait_for(std::chrono::milliseconds(timeout_ms)) == std::future_status::timeout) {
//            return 1e9;
//        }



        // Получение результата
        int search_result = search(x, m, y, n);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

        // Возвращение времени выполнения в миллисекундах
        return duration.count();
    }
};

#endif // ALGORITHM_H
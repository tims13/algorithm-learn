#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <list>
#include <vector>
#include <exception>
#include <semaphore.h>

using namespace std;

template<typename T>
class ThreadPool {
private:
    int m_thread_number;
    int m_max_request;
    pthread_t* m_threads;
    list<T*> m_workqueue;
    pthread_mutex_t m_queuelocker;
    sem_t m_queuestat;
    bool m_stop;

public:
    ThreadPool(int thread_number = 8, int max_requests = 1000) :
         m_thread_number(thread_number), m_max_request(max_requests), m_stop(false)
    {
        if (thread_number <= 0 || max_requests <= 0) {
            throw exception();
        }
        /* init lockers */
        if (pthread_mutex_init(&m_queuelocker, NULL) != 0) {
            throw exception();
        }
        if (sem_init(&m_queuestat, 0, 0) != 0) {
            throw exception();
        }
        m_threads = new pthread_t[m_thread_number];
        if (!m_threads) {
            throw exception();
        }
        for (int i= 0; i< thread_number; i++) {
            if (pthread_create(m_threads+i, NULL, worker, this) != 0) {
                delete [] m_threads;
                throw exception();
            }
            if (pthread_detach(m_threads[i])) {
                delete [] m_threads;
                throw exception();
            }
        }
    }
    
    ~ThreadPool() {
        delete [] m_threads;
        m_stop = true;
        
    }
    
    bool append(T* request) {
        pthread_mutex_lock(&m_queuelocker);
        if (m_workqueue.size() > m_max_request) {
            pthread_mutex_unlock(&m_queuelocker);
            return false;
        }
        m_workqueue.push_back(request);
        pthread_mutex_unlock(&m_queuelocker);
        sem_post(&m_queuestat);
        return true;
    }

private:    
    static void* worker(void* arg) {
        ThreadPool* pool = (ThreadPool*)arg;
        pool->run();
        return pool;
    }
    
    void run() {
        while (!m_stop) {
            sem_wait(&m_queuestat);
            pthread_mutex_lock(&m_queuelocker);
            if (m_workqueue.empty()) {
                pthread_mutex_unlock(&m_queuelocker);
                continue;
            }
            T* request = m_workqueue.front();
            m_workqueue.pop_front();
            pthread_mutex_unlock(&m_queuelocker);
            if (!request) {
                continue;
            }
            request->process();
        }
    }
};

class Node {
public:
    int num;
    Node(int n = 0) {
        num = n;
    }
    void process() {
        printf("process %d\n", num);
    }
};

int main() {
    ThreadPool<Node> *pool = NULL;
    pool = new ThreadPool<Node>(8,1000);
    for (int i= 0; i< 100; i++) {
        pool->append(new Node(i));
    }
    printf("APPEND FINISHED");
    cin.get();
    return 0;
}

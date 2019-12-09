#ifndef __UTILS_MULTITHREAD_H__
#define __UTILS_MULTITHREAD_H__
#include <functional>
#include <unistd.h>
#include <vector>
#include <map>
//#include <thread>
#include <cassert>
#include <pthread.h>
#include <stdlib.h>
/**********************************************************
 *
 *
 *  MultiThread class 
 *
 * Not guarentee thread safe , no lock.
 *
 * You must follow below usage :
 *  0. Call *Init* to initialize.
 *  1. Call *RegisterSignal* to register all signals 
 *        you need to handle and there work function.
 *  2. Call *Run* to start all threads.
 *  3. Call *SendWorkSignal* to send signal for all threads.
 *  4. Call *Quit* to kill all threads.
 *
 * *******************************************************/
struct RunFunc
{
    typedef void (*SignalCB)(int);
    RunFunc(int id,std::map<int,SignalCB> &cb,bool &q)
        : thread_id(id)
        , signal(SignalEnd)
        , cb_map(cb)
        , quit_flag(q){}

    RunFunc(const RunFunc& r)
        : thread_id(r.thread_id)
        , signal(r.signal)
        , cb_map(r.cb_map)
        , quit_flag(r.quit_flag) {}

    int thread_id ;
    int signal;
    const static int SignalEnd = 0;

    
    std::map<int, SignalCB> & cb_map;
    bool & quit_flag ;
    void* operator()(void *) 
    {

        while(true)
        {
            if(signal != SignalEnd)
            {
                if(cb_map.find(signal) != cb_map.end())
                {
                    cb_map.at(signal)(thread_id);
                }
                signal= SignalEnd;
            }
            if(quit_flag)
                break;
            usleep(1);
        }
        return NULL;
    }
};
static void * ARun(void *f )
{
    RunFunc * ff = (RunFunc*)f;
    return (*ff)(0);
}

struct MultiThread
{
    MultiThread()
        :thread_num(-1)
        ,global_signal(0)
        ,quit_flag(false)
    {
    };
    void Init(int t_num)
    {
        thread_num = t_num;
        allRuns =(RunFunc*) malloc(sizeof(RunFunc)*thread_num);
        all_thread.resize(t_num);
        for( int i = 0; i< thread_num ; i++ )
        {
            new (&allRuns[i])RunFunc(i,cb_map,quit_flag);
        }
    }

    //typedef std::function<void(int)> SignalCB;
    void RegisterSignal(int s, RunFunc::SignalCB c)
    {
        cb_map[s] = c;
    }

    void SendWorkSignal(int s)
    {
        for( int i = 0 ; i< thread_num; i++)
        {
            allRuns[i].signal = s;
        }
        WaitSignalEnd();
    }


    void Run()
    {
        for(int i = 0 ; i<thread_num; i++)
        {
            /*all_thread.emplace_back(std::thread([i,this](){
              this->RunRoutine(i);
              }));*/
            pthread_create(&all_thread[i], NULL, ARun, &allRuns[i]);
        }
    }

    void Quit()
    {
        quit_flag = true;
        for(size_t i = 0 ; i < all_thread.size() ; i++)
        {
            pthread_join(all_thread[i],NULL);
        }
    }
    private:
        int thread_num;
        int global_signal;
        bool quit_flag;
        std::map<int,RunFunc::SignalCB> cb_map;
        std::vector<pthread_t> all_thread;
        RunFunc *allRuns;
    private:
        void WaitSignalEnd()
        {
            while(true)
            {
                usleep(10);
                int i = 0;
                for(i = 0; i< thread_num ; i++)
                {
                    if(allRuns[i].signal != RunFunc::SignalEnd)
                        break;
                }
                if(i== thread_num)
                    break;
            }
        }
};


#endif //__UTILS_MULTITHREAD_H__

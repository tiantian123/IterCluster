#ifndef TIMER_H
#define TIMER_H
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <sys/time.h>
#include <unordered_map>
#include <pthread.h>
#include <stdlib.h>
#include <mutex>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>

class Timer
{
    public:
        Timer(std::string s, bool silent = false) : m_desc(s), m_silent(silent)
        {
            reset();
        }

        ~Timer()
        {
            if(!m_silent)
                printf("[timer - %s] wall clock: %.2lfs CPU: %.2lfs\n", m_desc.c_str(), getElapsedWallTime(), getElapsedCPUTime());
        }

        double getElapsedWallTime() const
        {
            timeval now;
            gettimeofday(&now, NULL);
            return (now.tv_sec - m_wallStart.tv_sec) + (double(now.tv_usec - m_wallStart.tv_usec) / 1000000);
        }

        double getElapsedCPUTime() const
        {
            double now = clock();
            return (now - m_cpuStart) / CLOCKS_PER_SEC;
        }

        void reset() { gettimeofday(&m_wallStart, NULL); m_cpuStart = clock(); }

    private:
        std::string m_desc;

        // Track the wall-clock and CPU times
        // CPU time includes all threads
        timeval m_wallStart;
        double m_cpuStart;

        bool m_silent;
};

#endif // TIMER_H

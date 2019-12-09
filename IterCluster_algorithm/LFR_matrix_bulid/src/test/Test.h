#ifndef __LFR_TEST_TEST_H__
#define __LFR_TEST_TEST_H__

#include "Timer.h"
#include "Check.h"
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <stdlib.h>
#include "Define.h"
#include "Global.h"
struct Test
{
    typedef  void (*testFunc)() ;

    typedef std::vector<testFunc> TestVec;

    typedef std::map<std::string , TestVec *> TestMap;

    static TestMap & the_map()
    {
        static TestMap themap;
        return themap;
    }

    static void TRun(const std::string &name ,TestVec * v)
    {
        the_map()[name] = v;
    }

    static void RunAllTest()
    {
        for(TestMap::iterator i = the_map().begin() ; i != the_map().end() ; i++) //& a: the_map())
        {
            RunVec( i->first,*i->second);
        }
    }
    static void RunTest(std::string module_name)
    {
        TestMap::iterator it = the_map().find(module_name);
        if(it != the_map().end() )
        {
            RunVec(it->first, *(it->second));
        }
    }
    private:
        static void RunVec(std::string name ,TestVec & v)
        {
            std::cout<<"---------------- Start test module "<<name<<" ---------------"<<std::endl;
            for(TestVec::iterator i = v.begin() ; i!=v.end() ; i++)
            {
                (*i)();
            }
            std::cout<<"---------------- End   test module "<<name<<" ---------------"<<std::endl<<std::endl;
        }
};

#define TEST_MODULE_INIT(name) \
    static Test::TestVec & get_module()\
    {\
        static Test::TestVec * thevec = nullptr;\
        if(thevec == nullptr)\
        {\
            thevec = new Test::TestVec();\
            Test::TRun(#name,thevec);\
        }\
        return *thevec;\
    }

#define TEST(name) \
    void name(); \
    namespace{\
        static void tmp_##name##_f(){\
                    Timer t(#name);\
                    name();\
        }\
        struct test_##name{\
            test_##name(){\
                get_module().push_back( tmp_##name##_f);\
            }\
        };\
        static test_##name tmp_##name;\
    }\
    void name()


#endif //__LFR_TEST_TEST_H_

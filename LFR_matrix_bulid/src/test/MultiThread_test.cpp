#include "MultiThread.h"
#include "Test.h"
#include <iostream>

TEST_MODULE_INIT(MultiThread);

static int aa[] = { 0 , 0 , 0};
void Incr(int i)
{
    aa[i]++;
}
void Decr(int i)
{
    aa[i]--;
}
void print(int i)
{
std::cout<<"from thread"<<i<<" value"<<aa[i]<<std::endl;
}
TEST(MultiT_Incr)
{
    MultiThread a;
    a.Init(3);
    a.RegisterSignal(1,Incr);
    a.RegisterSignal(2,Decr);
    a.RegisterSignal(3,print);

    a.Run();
    a.SendWorkSignal(1);
    a.SendWorkSignal(3);
    a.SendWorkSignal(1);
    a.SendWorkSignal(3);
    a.SendWorkSignal(2);
    a.SendWorkSignal(3);
    a.SendWorkSignal(1);
    a.SendWorkSignal(3);
    a.Quit();
}

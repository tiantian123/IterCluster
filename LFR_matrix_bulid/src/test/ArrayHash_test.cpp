#include "Test.h"
#define UTEST 1
#include "ArrayHash.h"
#include <set>

TEST_MODULE_INIT(ArrayHash)
/*
struct TestInit
{
    TestInit(): value(0) {}
    TestInit(int i ):value(i){}
    TestInit(const TestInit & i):value(i.value) {}

    operator size_t()const{
        return value;
    }

    TestInit & operator=(const TestInit & t)
    {
        value = t.value;
        return *this;
    }

    bool operator ==(const TestInit & t)
    {
        return value == t.value;
    }

    void UnSet() { value = 0 ;} 

    bool IsSetted() const { return value != 0 ; }

        int value;
};*/

typedef ArrayHash<int , int> TestArrayHash;

TEST(ArrayHash_AllocTest)
{
    TestArrayHash th(1000);
    CHECK( (0x1UL <<10) , th.size);
    int exp[2000];
    for( int i = 1; i < 2000 ; i ++ )
    {
        exp[i] = i ;//random();
        th[i].value = exp[i];
    }
    CHECK(  (0x1UL <<11) , th.size);
}
static    int exp[2000];

void ttt(TestArrayHash::slot & curr) {
    for( int i = 1; i < 1000 ; i ++ )
    {
        if(exp[i] == curr.value)
            exp[i] = 0;
    }
};
TEST(ArrayHash_SaveData_test)
{
    TestArrayHash th(1000);
    for( int i = 1; i < 2000 ; i ++ )
    {
        exp[i] = random();
        th[i].value = exp[i];
    }
    th.Foreach(ttt);
    for( int i = 1; i < 1000 ; i ++ )
        CHECK(0,exp[i])
}

TEST(NormalHash_1M)
{
    std::set<int> tt;
    const int  size1 = 1000000;
     for( int i = 1; i < size1; i ++ )
    {
        tt.insert((random()));
    }
    size_t a = 0;
    for(std::set<int>::iterator b =tt.begin() ; b != tt.end(); b++)
    {
        a+=int(*b);
    }
    std::cout<<a<<std::endl;
}


TEST(ArrayHash_SaveData_Speed_1M)
{
    TestArrayHash th(1000);
    const int  size1 = 1000000;
    for( int i = 1; i < size1; i ++ )
    {
        th[random()].value = random();
    }

}

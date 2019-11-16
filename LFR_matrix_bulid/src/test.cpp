#include  "Test.h"


static void print_usage()
{
    std::cout<<"LFR-matrix-test [command]"<<std::endl;
    std::cout<<"                no command means run all test module."<<std::endl<<std::endl;
    std::cout<<"                command can be :"<<std::endl;
    std::cout<<"                                 help"<<std::endl;

    for(Test::TestMap::iterator i =  Test::the_map().begin() ; i != Test::the_map().end() ; i++)
    {
        std::cout<<"                                 "<<i->first<<std::endl;
    }
}

int main(int argc, char ** argv)
{
    
    if(argc < 2)
    {
        Test::RunAllTest();
        return 0;
    }
    std::string command(argv[1]);
    if(command == "help")
        print_usage();
    else
        Test::RunTest(command);
    return 0;
}

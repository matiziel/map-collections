#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <map>
#include <ctime>
#include <vector>
#include <chrono>

#include "TreeMap.h"
#include "HashMap.h"

namespace 
{

    template <typename K, typename V>
	//using Map = aisdi::HashMap<K, V>;
    using Map = aisdi::TreeMap<K, V>;

    float test_remove(size_t size)
    {
        Map<int, int> my_col;
        for (unsigned int i = 0; i < size; ++i)
            my_col[i] = i;
        auto time_begin = std::chrono::system_clock::now();
		my_col.remove(size/4);
        auto time_end = std::chrono::system_clock::now();
        return (time_end-time_begin).count();
    }

    float test_insert(size_t size)
    {
        Map<int, int> my_col;
        
        for (unsigned int i = 0; i < size; ++i) 
        {
            if( i == size/4)
                continue;
            my_col[i] = i;
        }
        auto time_begin = std::chrono::system_clock::now();
        my_col[size/4] = 10;
        auto time_end = std::chrono::system_clock::now();
        return (time_end - time_begin).count();
    }

    float test_find(size_t size)
    {
        Map<int, int> my_col;
        for (unsigned int i = 0; i < size; ++i) 
            my_col[i] = i;
        auto time_begin = std::chrono::system_clock::now();
        my_col.find(size - 20);
        auto time_end = std::chrono::system_clock::now();
        return (time_end - time_begin).count();
    }




    void perfomTest() 
    {
        std::vector<unsigned int> sizes{ 100, 5000, 50000, 500000, 5000000, 50000000};

        for (unsigned int size : sizes)
	    {
		    std::cout<< "Operacja usuwania, liczba elementów: "<<size<<" w czasie: "<< test_remove(size)<<std::endl;
	
        }
        std::cout<<std::endl;
        for (unsigned int size : sizes)
	    {
		    
		    std::cout<< "Operacja wstawiania, liczba elementów: "<<size<<" w czasie: "<< test_insert(size)<<std::endl;	
        }
        std::cout<<std::endl;
        for (unsigned int size : sizes)
	    {
		    
		    std::cout<< "Operacja wyszukiwania, liczba elementów: "<<size<<" w czasie: "<< test_find(size)<<std::endl;	
        }
    }
}

int main(int argc, char** argv)
{
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;
    std::cout << "Testing for " << repeatCount << " iterations..." << std::endl;
    std::cout << "Testy dla hashmapy: "<<std::endl;
    //std::cout << "Testy dla treemapy: "<<std::endl;

    for (std::size_t i = 0; i < repeatCount; ++i) 
    {
        perfomTest();
    }
    /*Map<int, int> mymap;
    for(int i = 0; i<10; ++i)
        mymap[i]=i;
    for(auto it = mymap.begin(); it != mymap.end(); ++it)
        std::cout<<it->second<<std::endl;
    
    mymap.remove(3);
    mymap.remove(7);
    for(auto it = mymap.begin(); it != mymap.end(); ++it)
        std::cout<<it->second<<std::endl;*/


    return 0;
}

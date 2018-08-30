##C++引用指针代码案例
  #include <stdio.h>
  #include <map>

  int main() 
  { 
    std::map<int, int*> myMap;
    myMap.insert(std::make_pair(1, new int(1)));
    myMap.insert(std::make_pair(2, new int(2)));
    myMap.insert(std::make_pair(3, new int(3)));


    std::map<int, int*>::iterator it = myMap.find(2);
    int*& ref = it->second;  //修改成int* ref = it->second;即没问题
    printf("ref: %d \n", *ref);

    myMap.erase(it);


    myMap.insert(std::make_pair(4, new int(4)));
    for(std::map<int, int*>::iterator it = myMap.begin(); it != myMap.end(); it++)
    {
    printf("k: %d, v: %d\n", it->first, *it->second);
    }

    printf("ref: %d \n", *ref);

    return 0;
  }

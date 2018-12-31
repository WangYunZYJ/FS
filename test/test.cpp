//#include <memory>
//#include "inode/inode.h"
//#include "io/io.h"
//#include <cstdio>
//#include <iostream>
//#include "volume/volume.h"
//
//using namespace std;
//using namespace wyfs;
//#define BUFFER_SIZE 1<<20
//char buffer[BUFFER_SIZE];
//int main()
//{
////    FILE* file = fopen("/home/wy/FS/disk", "wb");
////    for(int i = 0; i < 64; ++i){
////        fwrite(buffer,BUFFER_SIZE,1,file);
////    }
////    cout << "Finished!\n";
//    auto _ = volume::get_instance();
//    cout << _.get() << endl;
//    auto __ = volume::get_instance();
//    cout << __.get() << endl;
//}
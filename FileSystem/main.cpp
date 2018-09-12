//
//  3.cpp
//  os1
//
//  Created by JHZ on 2018/9/1.
//  Copyright © 2018年 j. All rights reserved.
//

#include <iostream>
#include "fileSystem.h"

using namespace std;

int main(int argc, char* argv[])
{
    FileSystem file(argv[1]);
    file.init();
    cout << "Hello world!" << endl;
    return 0;
}

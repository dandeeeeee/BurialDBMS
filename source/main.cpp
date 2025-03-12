#include "raylib.h"
 
#if defined(_WIN32)           
    #define NOGDI            
    #define NOUSER           
#endif

#include <Windows.h> 

#if defined(_WIN32)           
    #undef far
#endif

#include "cpr/cpr.h"
#include "json.hpp"
#include <iostream>

using namespace std;    

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}

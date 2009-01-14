#include <stdio.h>
#include "thread.h"

void foo(int s);

int main() {
    //Initiliaze the library
    twine_init();

    twine_create(foo, 23);
    twine_create(foo, 33);

    

}

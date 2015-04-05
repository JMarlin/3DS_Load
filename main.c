
#include "3ds.h"

int main(int argc, char *argv[]) {                        
                           
    obj3ds *object = load3ds("test.3ds");
    jsonObj(object, "test.json");
}

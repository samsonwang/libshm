
#include <cstdio>
#include "SharedMemKeyGen.h"
#include "SharedMemOper.h"


#define UNUSED_ARGS(x)     (void)x

int main(int argc, char *argv[])
{
    UNUSED_ARGS(argc);
    UNUSED_ARGS(argv);
    
    printf("example: shm create\n");

    CSharedMemKeyGen objKeyGen;
    objKeyGen.SetParam("./example_shmkey", 1);
    objKeyGen.GenerateKey();
    

    CSharedMemOper objShmOper;
    objShmOper.SetSharedMemKey(objKeyGen.GetKey());
    objShmOper.CreateSharedMem(200);

    getchar();
    return 0;
}




#include <cstdio>
#include "SharedMemKeyGen.h"
#include "SharedMemOper.h"

#define UNUSED_ARGS(x)     (void)x

int main(int argc, char *argv[])
{
    UNUSED_ARGS(argc);
    UNUSED_ARGS(argv);

    printf("example: shm attach\n");

    CSharedMemKeyGen objKeyGen;
    objKeyGen.SetParam("./example_shmkey", 1);
    objKeyGen.GenerateKey();
    

    CSharedMemOper objShmOper;
    objShmOper.SetSharedMemKey(objKeyGen.GetKey());
    objShmOper.AttachSharedMem();

    getchar();
    return 0;
}



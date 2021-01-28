#include "hwcart.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

#ifdef EPYC
#define NLEVELS 6
    hwcart_split_t domain[NLEVELS] = {
                               HWCART_MD_HWTHREAD,
                               HWCART_MD_CORE,
                               HWCART_MD_L3CACHE,
                               HWCART_MD_NUMA,
                               HWCART_MD_SOCKET,
                               HWCART_MD_NODE
    };
    int topo[3*NLEVELS] = {
                           1, 1, 1, // thread
                           1, 1, 1, // core grid
                           2, 2, 1, // l3cache grid
                           1, 1, 4, // numa grid
                           1, 1, 1, // socket grid
                           1, 1, 1, // node grid
    };
#else
#define NLEVELS 6
    hwcart_split_t domain[NLEVELS] = {
                               HWCART_MD_HWTHREAD,
                               HWCART_MD_CORE,
                               HWCART_MD_L3CACHE,
                               HWCART_MD_NUMA,
                               HWCART_MD_SOCKET,
                               HWCART_MD_NODE
    };
    int topo[3*NLEVELS] = {
                           2, 1, 1, // hwthread grid
                           1, 2, 2, // core grid
                           1, 1, 1, // l3cache grid
                           1, 1, 1, // numa grid
                           1, 1, 1, // socket grid
                           1, 1, 1, // node grid
    };
#endif

    int comm_rank, comm_size;
    hwcart_order_t order = HWCartOrderXYZ;
    hwcart_topo_t hwtopo;
    MPI_Comm hwcart_comm;

    if(argc>1){
      order = atoi(argv[1]);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if(hwcart_init(&hwtopo)){
        MPI_Finalize();
        exit(1);
    }
    
    if(!hwcart_create(hwtopo, MPI_COMM_WORLD, NLEVELS, domain, topo, order, &hwcart_comm)){
        hwcart_print_rank_topology(hwtopo, hwcart_comm, NLEVELS, domain, topo, order);
        hwcart_free(&hwtopo, &hwcart_comm);
    }

    MPI_Finalize();
}

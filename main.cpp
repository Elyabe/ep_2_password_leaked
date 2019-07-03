#include<bits/stdc++.h> 
#include "sha1.h"
#include<mpi.h>
#include<sys/time.h>

using namespace std; 

#define TAM_MAX 5
#define ROOT 0
#define TAG 0

double MyClock() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}


int main(int argc, char *argv[])
{
    double inicio = MyClock();
    
    if ( argc != 2 )
    {
        printf("Bad leck :(! \n");
        exit(-1);
    }

    /* code */
	char caracteres[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; 
    // char caracteres[] = "ae0123456789";    
    // char caracteres[] = "abcdefghijklmnopqrstuvwxyz0123456789";    
    char pwd_decrypt[255];
    strcpy(pwd_decrypt,argv[1]);
    
   
    long T = pow( strlen(caracteres), TAM_MAX), j, k, l, m, n, CH_LEN = strlen(caracteres), NUM_PROCESSOS = 10;
    char sc[TAM_MAX+1];
    sc[TAM_MAX] = '\0';
    
    
    int  rank, size, L = 1;
    bool achou;
    // const char p[] = "a100e";
    // cout<< sha1(p) <<endl;
    // exit(0);

    MPI_Status stats;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    achou = false;

    /*if ( rank == ROOT )
    {
        char recv[6];
        
        for ( k = 1; k < size ; ++k ) {
            MPI_Recv(&achou, 1, MPI_C_BOOL, k, TAG, MPI_COMM_WORLD, &stats);
            
            if ( achou )
            {
                printf("Processo %ld achou:\n", k);
                MPI_Abort(MPI_COMM_WORLD, 0);
                break;
            }    
        }

    } else */
    {
            
            achou = false;

            for ( L = 1; !achou && L <= TAM_MAX; L++ )
            {
                const long long T = (long long)pow( strlen(caracteres), L);
                const long CH_LEN_POT[] = { 0, (long)pow(CH_LEN, L-1), (long)pow(CH_LEN, L-2), (long)pow(CH_LEN, L-3), CH_LEN };
                printf("Processo %d L = %d \n", rank,  L );


                
                // for ( long i = rank-1; i < T; i += size-1)
                for ( long i = rank; i < T; i += size)
                {
                    j = i / CH_LEN_POT[1];
                    if ( L >= 2)
                    {   
                        k = i % CH_LEN_POT[1]  / CH_LEN_POT[2];
                        
                        if ( L >= 3 )
                        {   
                            l =  i % CH_LEN_POT[1] % CH_LEN_POT[2] / CH_LEN_POT[3];
                            m =  i % CH_LEN_POT[1] % CH_LEN_POT[2] % CH_LEN_POT[3] / CH_LEN_POT[4];
                            n =  i % CH_LEN_POT[1] % CH_LEN_POT[2] % CH_LEN_POT[3] % CH_LEN_POT[4]; 
                        }
                    }

                    // printf("%d | %d | %d | %d | %d | %d  \n ", i,j,k,l,m, n);

                    long y[] = { j,k,l,m,n }, z;
                    
                    for ( z = 0; z < L; ++z )
                    {    
                        sc[z] = caracteres[y[z]];
                    }

                    sc[z] = '\0';

                    // printf("%s \n", sc );

                    if ( !strcmp( sha1(sc).c_str(), pwd_decrypt) )
                    {
                        printf("%lf : Processo %d achou %s com L = %d \n", (MyClock()-inicio)/CLOCKS_PER_SEC,  rank, sc, L );
                        achou = true;
                        // MPI_Bcast(&achou, 1, MPI_C_BOOL, rank, MPI_COMM_WORLD);
                        MPI_Abort(MPI_COMM_WORLD, 0);
                        break;
                    }    
                    
                }
                
                // MPI_Send(&achou, 1, MPI_C_BOOL, ROOT, TAG, MPI_COMM_WORLD); 
                
            }

    }



    MPI_Finalize();



    return 0;
}

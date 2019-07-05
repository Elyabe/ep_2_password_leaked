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
    char caracteres[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", pwd_decrypt[255];
    // char caracteres[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789", pwd_decrypt[255];
    strcpy(pwd_decrypt,argv[1]);
    
    const long CH_LEN = strlen(caracteres);
    char sc[TAM_MAX+1];
    
    int  rank, size, L = 1, done = -1;
    bool achou, res;

    MPI_Status stats;
    MPI_Request req;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    res = achou = false;

    if ( rank == ROOT )
    {
        while( !res )
        {
            if(done != 0)
            {
                MPI_Irecv(&res, 1, MPI_C_BOOL, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
                done = 0;
            }
                MPI_Test(&req, &done, &stats);

                if (done != 0) { 
                    MPI_Recv(&sc, TAM_MAX, MPI_CHAR, stats.MPI_SOURCE, TAG, MPI_COMM_WORLD, &stats);
                    
                    printf("Processo : %d ==: senha %s \n", stats.MPI_SOURCE, sc);

                    double tempoSeq = (MyClock()-inicio)/CLOCKS_PER_SEC;
                    cout << fixed << setprecision(25);
                    // cout << "  Valor esperado: " << M_PI << endl;
                    // cout << "   Erro Relativo: " << fabs(x - M_PI)/M_PI << endl;
                    printf("     Tempo total: %.5lf\n", tempoSeq);
                    // cout << "   SpeedUp: " << 90.4/tempoSeq << endl;
                    // cout << "   Eficiencia: " << 100*(90.4/tempoSeq)/16 << endl;

                    done = -1;
                    MPI_Abort(MPI_COMM_WORLD, 0);
                    break;
                }                

        } 

    } else 
    {
            long y[5], q, CH_LEN_POT[TAM_MAX+1];
            long long QTD_POSSIB;

            for ( L = 1; !achou && L <= TAM_MAX; L++ )
            {
                QTD_POSSIB = (long long)pow( strlen(caracteres), L);
                
                for ( q = 1; q <= TAM_MAX; q++ )
                    CH_LEN_POT[q] = (long)pow(CH_LEN, L-q);
                
                // CH_LEN_POT[1] = (long)pow(CH_LEN, L-1);
                // CH_LEN_POT[2] = (long)pow(CH_LEN, L-2);
                // CH_LEN_POT[3] = (long)pow(CH_LEN, L-3);
                // CH_LEN_POT[4] =  CH_LEN;
                // CH_LEN_POT[5] =  1;

                for ( long i = rank-1; i < QTD_POSSIB ; i += size-1)
                {
                    // Define os indices i,j,k,l
                    y[0] = i;
                    for ( q = 1 ; q < L; ++q )
                        y[q] = y[q-1] % CH_LEN_POT[q];

                    // Forma a palavra
                    for ( q = 0 ; q < L; ++q )
                        sc[q] = caracteres[y[q] / CH_LEN_POT[q+1]];    
                    sc[q] = '\0';


                    if ( !strcmp( sha1(sc).c_str(), pwd_decrypt) )
                    {
                        // printf("%lf : Processo %d achou %s com L = %d \n", (MyClock()-inicio)/CLOCKS_PER_SEC,  rank, sc, L );
                        achou = true;

                        // Informa ao raiz que achou
                        MPI_Send(&achou, 1, MPI_C_BOOL, ROOT, TAG, MPI_COMM_WORLD); 
                        // Envia a senha descriptografada
                        MPI_Send(&sc, TAM_MAX, MPI_CHAR, ROOT, TAG, MPI_COMM_WORLD); 
                        // MPI_Abort(MPI_COMM_WORLD, 0);
                        // MPI_Finalize();
                        // exit(0);
                        break;
                    } 
                }
            }

            // printf("Process %d terminated\n", rank );

    }

    MPI_Finalize();



    return 0;
}

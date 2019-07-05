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
        printf("Bad leck :(! Qual eh a senha \n");
        exit(-1);
    }

    /* code */
    char caracteres[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", pwd_decrypt[80], senha[TAM_MAX+1];
    // char caracteres[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789", pwd_decrypt[255];
    strcpy(pwd_decrypt,argv[1]);
    
    const long CH_LEN = strlen(caracteres);
    int  rank, size, TAM_SENHA, RECEBEU_MSG = 0;
    bool achou = false;

    MPI_Status stats;
    MPI_Request requisicao_msg;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    

    if ( rank == ROOT )
    {

        // Aguarda mensagem de qualquer processo (o que achar primeiro)
        MPI_Irecv(&senha, TAM_MAX, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &requisicao_msg);

        while( !RECEBEU_MSG )
            MPI_Test(&requisicao_msg, &RECEBEU_MSG, &stats);


        printf("Processo : %d ==: senha %s \n", stats.MPI_SOURCE, senha);

        const double tempoSeq = (MyClock()-inicio)/CLOCKS_PER_SEC;
        cout << fixed << setprecision(25);
        // cout << "  Valor esperado: " << M_PI << endl;
        // cout << "   Erro Relativo: " << fabs(x - M_PI)/M_PI << endl;
        printf("     Tempo total: %.5lf\n", tempoSeq);
        // cout << "   SpeedUp: " << 90.4/tempoSeq << endl;
        // cout << "   Eficiencia: " << 100*(90.4/tempoSeq)/16 << endl;

        MPI_Abort(MPI_COMM_WORLD, 0);

    } else {
            long long y[5], q, CH_LEN_POT[TAM_MAX+1], QTD_POSSIBILIDADES;

            for ( TAM_SENHA = 1; !achou && TAM_SENHA <= TAM_MAX; ++TAM_SENHA )
            {
                QTD_POSSIBILIDADES = (long long)pow( CH_LEN, TAM_SENHA);
                
                // Define os valores para calculos de indices necessarios o tamanho atual de senha
                for ( q = 1; q <= TAM_MAX; q++ )
                    CH_LEN_POT[q] = (long)pow(CH_LEN, TAM_SENHA-q);

                // Analisa todas as possibilidades
                for ( long long i = rank-1; i < QTD_POSSIBILIDADES ; i += size-1)
                {
                    // Define apenas os indices necessarios para o tamanho da senha calculada
                    y[0] = i;
                    for ( q = 1 ; q < TAM_SENHA; ++q )
                        y[q] = y[q-1] % CH_LEN_POT[q];

                    // Forma a palavra
                    for ( q = 0 ; q < TAM_SENHA; ++q )
                        senha[q] = caracteres[y[q] / CH_LEN_POT[q+1]];    
                    senha[q] = '\0';


                    if ( !strcmp( sha1(senha).c_str(), pwd_decrypt) )
                    {
                        achou = true;
                        MPI_Send(&senha, TAM_MAX, MPI_CHAR, ROOT, TAG, MPI_COMM_WORLD); 
                        break;
                    } 
                }
            }
    }

    MPI_Finalize();

    return 0;
}

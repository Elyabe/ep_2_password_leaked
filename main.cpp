#include<bits/stdc++.h> 
#include "sha1.h"
#include<mpi.h>
#include<sys/time.h>

using namespace std; 

#define TAM_MAX 5
#define ROOT 0
#define TAG 0

int main(int argc, char *argv[])
{
    double inicio = MPI_Wtime();
    
    if ( argc != 2 )
    {
        printf("Bad leck :(! Qual eh a senha \n");
        exit(-1);
    }

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

        const double tempoSeq = MPI_Wtime()-inicio;
        cout << fixed << setprecision(25);
        // cout << "  Valor esperado: " << M_PI << endl;
        // cout << "   Erro Relativo: " << fabs(x - M_PI)/M_PI << endl;
        printf("     Tempo total: %.10lf\n", tempoSeq);
        // cout << "   SpeedUp: " << 90.4/tempoSeq << endl;
        // cout << "   Eficiencia: " << 100*(90.4/tempoSeq)/16 << endl;

        MPI_Abort(MPI_COMM_WORLD, 0);

    } else {
            long long i[5], k, CH_LEN_POT[TAM_MAX+1], QTD_POSSIBILIDADES;

            for ( TAM_SENHA = 1; !achou && TAM_SENHA <= TAM_MAX; ++TAM_SENHA )
            {
                QTD_POSSIBILIDADES = (long long)pow( CH_LEN, TAM_SENHA);
                
                // Define os valores para calculos de indices necessarios o tamanho atual de senha
                for ( k = 1; k <= TAM_MAX; k++ )
                    CH_LEN_POT[k] = (long)pow(CH_LEN, TAM_SENHA-k);

                // Analisa todas as possibilidades
                for ( long long p = rank-1; p < QTD_POSSIBILIDADES ; p += size-1)
                {
                    // Define apenas os indices necessarios para o tamanho da senha calculada
                    i[0] = p;
                    for ( k = 1 ; k < TAM_SENHA; ++k )
                        i[k] = i[k-1] % CH_LEN_POT[k];

                    // Forma a palavra
                    for ( k = 0 ; k < TAM_SENHA; ++k )
                        senha[k] = caracteres[i[k] / CH_LEN_POT[k+1]];    
                    senha[k] = '\0';


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

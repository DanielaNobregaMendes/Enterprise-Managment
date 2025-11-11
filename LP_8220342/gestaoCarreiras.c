/**
 *  @brief inclusão de interfaces necessárias para o trabalho
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "funcionalidades.h"
#include "input.h"


/**
 * @brief Esta função mostra no ecrã os dados existentes no registo
 * 
 * @param carreira 
 */

void imprimirCarreiras(Carreira carreira) {


    printf("\tCódigo Departamento: %5d\n", carreira.codDepartamento);
    printf("\tCodigo Funcionario: %5d\n", carreira.codFuncionario);
    printf("\tData início: %02d/%02d/%04d\n", carreira.data_carreira.dia_inicio, carreira.data_carreira.mes_inicio, carreira.data_carreira.ano_inicio);
    printf("\tData Fim: %02d/%02d/%04d\n", carreira.data_carreira.dia_fim, carreira.data_carreira.mes_fim, carreira.data_carreira.ano_fim);

    puts(BARRA);

}

/**
 * @brief Esta função serve para saber qual a data maior
 * 
 * @param dia1 número do dia
 * @param mes1 número do mês
 * @param ano1 número do ano
 * 
 * @return -1 se a data maior é a 2, se retornar 1 quer dizer que a data1 é maior ou igual à data2
 */

int dataMaior(int dia1, int mes1, int ano1) {
    int dia2, mes2, ano2;
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    dia2 = tm.tm_mday;
    mes2 = tm.tm_mon + 1;
    ano2 = tm.tm_year + 1900;


    if (ano1 > ano2 || ano1 == ano2 && (mes1 > mes2 || (mes1 == mes2 && dia1 > dia2))) {
        return 1;
    } else if (ano1 < ano2 || ano1 == ano2 && (mes1 < mes2 || (mes1 == mes2 && dia1 < dia2))) {
        return -1;
    }
    return 1;
}

/**
 * @brief Esta função verifica se o número de dias entre 2 datas é menor ou igual a 30 e maior que 0
 * 
 * @param dia1 número do dia
 * @param mes1 número do mês
 * @param ano1 número do ano
 * 
 * @return -1 se o número de dias entre as duas data for maior que 30 ou menor que 0, ou retorna 1 se o número de dias estiver entre 30 e 0
 */

int dataContrato30Dias(int dia1, int mes1, int ano1) {
    int dia2, mes2, ano2;
    time_t start_time;
    time_t end_time;
    struct tm startDate;
    struct tm endDate;

    //Inicializar struct tm
    memset(&endDate, 0, sizeof (endDate));
    memset(&startDate, 0, sizeof (startDate));

    //data atual
    start_time = time(NULL);
    startDate = *localtime(&start_time);


    // data do contrato
    endDate.tm_year = ano1 - 1900;
    endDate.tm_mon = mes1 - 1;
    endDate.tm_mday = dia1;


    end_time = mktime(&endDate);
    start_time = mktime(&startDate);

    //cálculo dos dias
    double dt = difftime(end_time, start_time);
    int dias = round(dt / 86400);

    if (dias <= 30 && dias >= 0) {
        return 1;
    }

    return -1;
}

/**
 * @brief Esta função verifica se o funcionário existe nos registos dos Contratos
 * 
 * @param carreiras
 * @param codFuncionario
 * @return i se o funcionário existir e -1 se ele não existir
 */

int procurarFuncCarreira(Carreiras carreiras, int codFuncionario) {
    int i;

    for (i = 0; i < carreiras.contador; i++) {
        if (carreiras.carreiras[i].codFuncionario == codFuncionario)
            return i;
    }

    return -1;
}

/**
 * @brief Esta função verifica se o departamento existe nos registos
 * 
 * @param carreiras
 * @param codFuncionario
 * @return i se o departamento existir e -1 se ele não existir
 */
int procurarDepartCarreira(Carreiras carreiras, int codDepartamento) {
    int i;

    for (i = 0; i < carreiras.contador; i++) {
        if (carreiras.carreiras[i].codDepartamento == codDepartamento) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Esta função verifica se o ficheiro existe(rb), senão cria-o (wb)
 * Com a função fread é retornado o número de itens completos lidos pela função
 * 
 * @param carreiras
 * @param ficheiro
 */

void carregarCarreiras(Carreiras *carreiras, char *ficheiro) {
    int i, sucesso = 0;

    //Tenta abrir o ficheiro
    FILE *fp = fopen(ficheiro, "rb");
    if (fp != NULL) {
        //retorna o número de itens completos lidos pela função
        fread(&carreiras->contador, sizeof (int), 1, fp);

        if (carreiras->contador > 0) {
            carreiras->carreiras = (Carreira*) malloc(carreiras->contador * sizeof (Carreira));
            fread(carreiras->carreiras, sizeof (Carreira), carreiras->contador, fp);
            carreiras->tamanho = carreiras->contador;
            sucesso = 1;
        }
        fclose(fp);
    }

    if (!sucesso) {

        fp = fopen(ficheiro, "wb");
        if (fp != NULL) {

            carreiras->carreiras = (Carreira*) malloc(CARREIRAS_QUANT_INICIAL * sizeof (Carreira));
            carreiras->contador = 0;
            carreiras->tamanho = CARREIRAS_QUANT_INICIAL;
            fclose(fp);
        }
    }

}

/**
 * @brief Esta função desaloca a memória alocada 
 * 
 * @param carreiras
 */
void libertarCarreiras(Carreiras *carreiras) {
    free(carreiras->carreiras);
}

/**
 * @brief Esta função guarda o contador dos registos
 * 
 * @param contador
 * @param ficheiro
 */
void atualizarCarreirasContadorFX(int contador, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fwrite(&contador, sizeof (int), 1, fp);
    fclose(fp);
}

/**
 *@brief Esta função regista o contrato no ficheiro binário
 * 
 * @param carreiras
 * @param ficheiro
 */
void registarCarreirasFX(Carreiras carreiras, char *ficheiro) {
    atualizarCarreirasContadorFX(carreiras.contador, ficheiro);

    FILE *fp = fopen(ficheiro, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&carreiras.carreiras[carreiras.contador - 1], sizeof (Carreira), 1, fp);

    fclose(fp);
}

/**
 * @brief Verifica se o código do departamento e o código de funcionário que o utilizador inseriu existe
 * Verifica também se um funcionário que esteja já registado num contrato com a data do fim passada pode ser registado outra vez 
 * Insere um novo registo nos contratos
 * 
 * @param carreiras
 * @param funcionarios
 * @param departamentos
 * @return -1 se um dos códigos já existirem ou retorna o número do contador se os dados foram registados com sucesso
 */
int registarCarreira(Carreiras *carreiras, Funcionarios *funcionarios, Departamentos *departamentos) {
    int meses, codDepartamento, codFunc, codFuncCarreira;
    char estado[8] = "ativo";

    time_t t;
    struct tm *tmp;


    codDepartamento = obterInt(COD_MIN_DEPARTAMENTO, COD_MAX_DEPARTAMENTO, OBTER_COD_DEPART);
    if (procurarDepartamento(*departamentos, codDepartamento) != -1) {
        codFunc = obterInt(COD_MIN_FUNCIONARIO, COD_MAX_FUNCIONARIO, OBTER_COD_FUNCIONARIO);
        if (procurarFuncionario(*funcionarios, codFunc) != -1) {
            codFuncCarreira = procurarFuncCarreira(*carreiras, codFunc);
            if (codFuncCarreira != -1 && dataMaior(carreiras->carreiras[codFuncCarreira].data_carreira.dia_fim, carreiras->carreiras[codFuncCarreira].data_carreira.mes_fim, carreiras->carreiras[codFuncCarreira].data_carreira.ano_fim) == -1 || codFuncCarreira == -1) {


                carreiras->carreiras[carreiras->contador].codDepartamento = codDepartamento;

                carreiras->carreiras[carreiras->contador].codFuncionario = codFunc;
                meses = obterInt(MESES_CARREIRAS_MIN, MESES_CARREIRAS_MAX, "Insira os meses do contrato: ");

                //data do fim do contrato
                time(&t);
                //valor em segundos
                t += meses * 30 * 24 * 60 * 60;
                tmp = localtime(&t);


                carreiras->carreiras[carreiras->contador].data_carreira.ano_fim = tmp->tm_year + 1900;
                carreiras->carreiras[carreiras->contador].data_carreira.dia_fim = tmp->tm_mday;
                carreiras->carreiras[carreiras->contador].data_carreira.mes_fim = tmp->tm_mon + 1;

                //data atual, ou seja data de início do contrato
                t = time(NULL);
                tmp = localtime(&t);

                carreiras->carreiras[carreiras->contador].data_carreira.dia_inicio = tmp->tm_mday;
                carreiras->carreiras[carreiras->contador].data_carreira.mes_inicio = tmp->tm_mon + 1;
                carreiras->carreiras[carreiras->contador].data_carreira.ano_inicio = tmp->tm_year + 1900;

                return carreiras->contador++;
            }
        }
    }

    return -1;
}

/**
 * @brief Esta função aumenta o tamanho da memória para registar contratos
 * 
 * @param carreiras
 */
void expandirCarreiras(Carreiras *carreiras) {
    Carreira *temp = (Carreira*) realloc(carreiras->carreiras, sizeof (Carreira) * (carreiras->tamanho * 2));
    if (temp != NULL) {
        carreiras->tamanho *= 2;
        carreiras->carreiras = temp;
    }
}

/**
 * @brief Verifica se o número do contador e do tamanho são iguais, se sim chama a função expandirCarreiras()
 * Caso o contador seja menor que o tamanho verifica-se se o registo do contrato correu bem, caso não corresse retorna -1 e aparece uma mensagem (CARREIRAS_EXISTE)
 * Se o valor retornado for diferente de -1 chama-se a função registarCarreirasFX() para escrever no ficheiro o registo
 * 
 * @param carreiras
 * @param funcionarios
 * @param departamentos
 * @param ficheiro
 */
void registarCarreiras(Carreiras *carreiras, Funcionarios *funcionarios, Departamentos *departamentos, char *ficheiro) {
    if (carreiras->contador == carreiras->tamanho) {
        expandirCarreiras(carreiras);
    }

    if (carreiras->contador < carreiras->tamanho) {
        if (registarCarreira(carreiras, funcionarios, departamentos) == -1) {
            puts(CARREIRAS_EXISTE);
        } else {
            registarCarreirasFX(*carreiras, ficheiro);
        }
    } else {
        puts(CARREIRAS_LISTA_CHEIA);
    }
}

/**
 * @brief Esta função lista os funcionários de um departamento
 * Verifica também se o departamento existe pelo código do mesmo
 * 
 * @param carreiras
 */
void listarFuncionariosCarreiras(Carreiras carreiras) {
    int i;
    int codDepartamento;

    codDepartamento = obterInt(COD_MIN_DEPARTAMENTO, COD_MAX_DEPARTAMENTO, OBTER_COD_DEPART);

    if (carreiras.contador > 0) {
        if (procurarDepartCarreira(carreiras, codDepartamento) != -1) {
            printf("--------------- CONTRATOS ---------------\n");
            //printf("\n\n-------------------- CONTRATOS --------------------\n\n");
            //printf("Departamento\tFuncionario\tData Início\t\tData Fim\n\n");
            for (i = 0; i < carreiras.contador; i++) {
                if (carreiras.carreiras[i].codDepartamento == codDepartamento) {
                    imprimirCarreiras(carreiras.carreiras[i]);
                }
            }
        } else {
            puts(DEPART_NAO_EXISTE);
        }

    } else {
        puts(CARREIRAS_LISTA_VAZIA);
    }
}

/**
 * @brief Lista os contrato que estão a acabar ao fim de 30 dias
 * 
 * @param carreiras
 */

void listarContratosCarreiras(Carreiras carreiras) {
    int i;

    if (carreiras.contador > 0) {

        puts("");
        printf("\n\n-------------------- CONTRATOS --------------------\n");

        for (i = 0; i < carreiras.contador; i++) {
            if (dataContrato30Dias(carreiras.carreiras[i].data_carreira.dia_fim, carreiras.carreiras[i].data_carreira.mes_fim, carreiras.carreiras[i].data_carreira.ano_fim) == 1) {
                imprimirCarreiras(carreiras.carreiras[i]);
            }

        }

    } else {
        puts(CARREIRAS_LISTA_VAZIA);
    }
}

/**
 * @brief Esta função lê o ficheiro binário com os registos dos contratos e escreve num ficheiro de texto
 * Verifica se a data dos contratos é passada ou não, escrevendo depois no ficheiro de texto os contratos com data corrente
 * 
 * @param carreiras
 * @param ficheiroBin
 * @param ficheiroTxt
 * @return 
 */
int exportarAtualDF(Carreiras carreiras, char* ficheiroBin, char* ficheiroTxt) {
    FILE *fbin, *ftxt;
    int i;
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    if ((fbin = fopen(ficheiroBin, "rb")) == NULL)
        return 0; // sem sucesso

    if ((ftxt = fopen(ficheiroTxt, "wt")) == NULL) { // changed mode
        fclose(fbin);
        return 0; // sem sucesso
    }

    fprintf(ftxt, "Departamento\tFuncionario\tData Início\t\tData Fim\n");
    fprintf(ftxt, "---------------------------------------------------------------------------------------------------------------------------------\n");

    if (carreiras.contador > 0) {
        for (i = 0; i < carreiras.contador; i++) {
           
            if (dataMaior(carreiras.carreiras[i].data_carreira.dia_fim, carreiras.carreiras[i].data_carreira.mes_fim, carreiras.carreiras[i].data_carreira.ano_fim) == 1)
                fprintf(ftxt, "%d\t\t%d\t\t%02d/%02d/%04d\t\t%02d/%02d/%04d\n", carreiras.carreiras[i].codDepartamento, carreiras.carreiras[i].codFuncionario, carreiras.carreiras[i].data_carreira.dia_inicio, carreiras.carreiras[i].data_carreira.mes_inicio, carreiras.carreiras[i].data_carreira.ano_inicio, carreiras.carreiras[i].data_carreira.dia_fim, carreiras.carreiras[i].data_carreira.mes_fim, carreiras.carreiras[i].data_carreira.ano_fim);
        }
    } else {
        printf("Failure\n");
    }

    fclose(ftxt);
    fclose(fbin);

    return 1;
}

/**
 * @brief Esta função lê o ficheiro binário e escreve os contratos passados num ficheiro de texto
 * Verifica-se se a data é passada com a função dataMaior()
 * 
 * @param carreiras
 * @param ficheiroBin
 * @param ficheiroTxt
 * @return 
 */

int exportarOldDF(Carreiras carreiras, char* ficheiroBin, char* ficheiroTxt) // changed return value
{
    FILE *fbin, *ftxt;
    int i;
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    Funcionarios funcionarios;


    if ((fbin = fopen(ficheiroBin, "rb")) == NULL)
        return 0;

    if ((ftxt = fopen(ficheiroTxt, "wt")) == NULL) { // changed mode
        fclose(fbin);
        return 0;
    }

    fprintf(ftxt, "Departamento\tFuncionario\tData Início\t\tData Fim\n");
    fprintf(ftxt, "---------------------------------------------------------------------------------------------------------------------------------\n");

    if (carreiras.contador > 0) {
        for (i = 0; i < carreiras.contador; i++) {

            if (dataMaior(carreiras.carreiras[i].data_carreira.dia_fim, carreiras.carreiras[i].data_carreira.mes_fim, carreiras.carreiras[i].data_carreira.ano_fim) == -1)
                fprintf(ftxt, "%d\t\t%d\t\t%02d/%02d/%04d\t\t%02d/%02d/%04d\n", carreiras.carreiras[i].codDepartamento, carreiras.carreiras[i].codFuncionario, carreiras.carreiras[i].data_carreira.dia_inicio, carreiras.carreiras[i].data_carreira.mes_inicio, carreiras.carreiras[i].data_carreira.ano_inicio, carreiras.carreiras[i].data_carreira.dia_fim, carreiras.carreiras[i].data_carreira.mes_fim, carreiras.carreiras[i].data_carreira.ano_fim);
        }

    } else {
        printf(CARREIRAS_NAO_EXISTE);
    }

    fclose(ftxt);
    fclose(fbin);

    return 1;
}

/**
 * @brief Lista de todos os contratos registados
 * 
 * @param carreiras
 */
void listarCarreiras(Carreiras carreiras) {
    int i;

    if (carreiras.contador > 0) {
        puts("");
        printf("\n\n-------------------- CONTRATOS --------------------\n");
        printf("Departamento\tFuncionario\tData Início\t\tData Fim\n\n");
        for (i = 0; i < carreiras.contador; i++) {
            imprimirCarreiras(carreiras.carreiras[i]);
        }
    } else {
        puts(FUNC_LISTA_VAZIA);
    }
}

void imprimirCarreirasFunc(Carreira carreira) {


    printf("\tCodigo Funcionario: %5d\n", carreira.codFuncionario);
    puts(BARRA);

}

void departFuncAtivos(Carreiras carreiras, Funcionarios funcionarios, int codDepartamento) {
    int i, j;
    
    if (carreiras.contador > 0) {
        if (procurarDepartCarreira(carreiras, codDepartamento) != -1) {
            printf("--------------- FUNCIONÁRIOS ---------------\n");
            for (i = 0; i < carreiras.contador; i++) {
                if(carreiras.carreiras[i].codDepartamento==codDepartamento)
                    if (dataMaior(carreiras.carreiras[i].data_carreira.dia_fim, carreiras.carreiras[i].data_carreira.mes_fim, carreiras.carreiras[i].data_carreira.ano_fim) == 1)
                       imprimirCarreirasFunc(carreiras.carreiras[i]);
                }
            }
        } else {
            puts(DEPART_LISTA_VAZIA);
        }




}

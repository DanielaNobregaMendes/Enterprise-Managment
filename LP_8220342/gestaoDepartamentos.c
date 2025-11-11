/**
 *  @brief inclusão de interfaces necessárias para o trabalho
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "input.h"

/**
 * @brief Mostra no ecrã os dados existentes no registo dos Departamentos
 * 
 * @param departamento
 */
void imprimirDepartamentos(Departamento departamento) {

    printf("\tCódigo: %5d", departamento.codDepartamento);
    printf("\tNome: %s\n", departamento.nomeDepart);
    printf("\tFuncionário Responsável: %5d\n", departamento.codFuncRespomsavel);
    printf("\tEstado: %s\n", departamento.estado);
    puts(BARRA);

}

/**
 * @brief Esta função "apaga" o registo dos departamentos e colocam as variáveis com números(int, double) a 0 e as variáveis de texto (string) sem texto ("")
 * 
 * @param departamento
 */
void apagarDadosDepartamentos(Departamento *departamento) {
    departamento->codFuncRespomsavel = 0;
    departamento->codDepartamento = 0;
    strcpy(departamento->nomeDepart, "");
    strcpy(departamento->estado, "");

}

/**
 * @brief Esta função verifica se o departamento existe nos registos dos Departamentos
 * 
 * @param departamentos
 * @param codDepartamento
 * @return i se o departamento existir e -1 se ele não existir
 */
int procurarDepartamento(Departamentos departamentos, int codDepartamento) {
    int i;
    for (i = 0; i < departamentos.contador; i++) {
        if (departamentos.departamentos[i].codDepartamento == codDepartamento) {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Verifica se o funcionário responsável de um departamento já existe noutro departamento
 * 
 * @param departamentos
 * @param codFunc
 * @return i se o funcionário existir e -1 se ele não existir
 */
int procurarFuncResponsavel(Departamentos departamentos, int codFunc) {
    int i;
    for (i = 0; i < departamentos.contador; i++) {
        if (departamentos.departamentos[i].codFuncRespomsavel == codFunc) {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Esta função verifica se o ficheiro existe(rb), senão cria-o (wb)
 * Com a função fread é retornado o número de itens completos lidos pela função
 * 
 * @param departamentos
 * @param ficheiro
 */
void carregarDepartamentos(Departamentos *departamentos, char *ficheiro) {
    int i, sucesso = 0;

    //Tenta abrir o ficheiro
    FILE *fp = fopen(ficheiro, "rb");
    if (fp != NULL) {
        //retorna o número de itens completos lidos pela função
        fread(&departamentos->contador, sizeof (int), 1, fp);

        if (departamentos->contador > 0) {
            departamentos->departamentos = (Departamento*) malloc(departamentos->contador * sizeof (Departamento));
            fread(departamentos->departamentos, sizeof (Departamento), departamentos->contador, fp);
            departamentos->tamanho = departamentos->contador;
            sucesso = 1;
        }

        fclose(fp);
    }

    if (!sucesso) {
        fp = fopen(ficheiro, "wb");
        if (fp != NULL) {

            departamentos->departamentos = (Departamento*) malloc(DEPARTAMENTO_QUANT_INICIAL * sizeof (Departamento));
            departamentos->contador = 0;
            departamentos->tamanho = DEPARTAMENTO_QUANT_INICIAL;
            fclose(fp);
        }
    }


}

/**
 * @brief Esta função desaloca a memória alocada 
 * 
 * @param departamentos
 */
void libertarDepartamentos(Departamentos *departamentos) {
    free(departamentos->departamentos);
}

/**
 * @brief Esta função guarda o contador dos registos
 * 
 * @param contador
 * @param ficheiro
 */
void atualizarDepartContadorFX(int contador, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fwrite(&contador, sizeof (int), 1, fp);
    fclose(fp);
}

/**
 * @brief Esta função regista o departamento no ficheiro binário
 * 
 * @param departamentos
 * @param ficheiro
 */
void registarDepartFX(Departamentos departamentos, char *ficheiro) {
    atualizarDepartContadorFX(departamentos.contador, ficheiro);

    FILE *fp = fopen(ficheiro, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&departamentos.departamentos[departamentos.contador - 1], sizeof (Departamento), 1, fp);

    fclose(fp);
}

/**
 * @brief Esta função atualiza os dados de um departamento no ficheiro binário
 * 
 * @param departamentos
 * @param indice
 * @param ficheiro
 */
void editarDepartFX(Departamentos departamentos, int indice, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "rb+");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof (int), SEEK_SET);

    fseek(fp, sizeof (Departamento) * indice, SEEK_CUR);

    fwrite(&departamentos.departamentos[indice], sizeof (Departamento), 1, fp);
}

/**
 * @brief Esta função remove o registo do departamento no ficheiro binário
 * 
 * @param departamentos
 * @param ficheiro
 */
void removerDepartFX(Departamentos departamentos, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fwrite(&departamentos.contador, sizeof (int), 1, fp);
    fwrite(departamentos.departamentos, sizeof (Departamento), departamentos.contador, fp);
}

/**
 * @brief Verifica se o código do funcionário que o utilizador inseriu existe com a função procurarFuncionario()
 * Verifica também se o funcionário já é responsável por outro departamento com a função procurarFuncResponsavel()
 * O código do departamento é dado pelo contador do registo dos departamento
 * No fim insere um novo registo em Departamentos e o contador aumenta por 1
 * 
 * @param departamentos
 * @param funcionarios
 * @return -1 se o código já existir ou retorna o número do contador se os dados foram registados com sucesso
 */
int registarDepartamento(Departamentos *departamentos, Funcionarios *funcionarios) {
    int codDepartamento, codFunc;
    codDepartamento = 1000 + departamentos->contador;

    codFunc = obterInt(COD_FUNC_MIN_DEPARTAMENTO, COD_FUNC_MAX_DEPARTAMENTO, OBTER_COD_FUNC_RESPONSAVEL);

    if (procurarFuncionario(*funcionarios, codFunc) != -1) {
        if (procurarFuncResponsavel(*departamentos, codFunc) == -1) {
            departamentos->departamentos[departamentos->contador].codDepartamento = codDepartamento;
            departamentos->departamentos[departamentos->contador].codFuncRespomsavel = codFunc;
            lerString(departamentos->departamentos[departamentos->contador].nomeDepart, DEPARTAMENTO_MAX_TAM, OBTER_NOME_DEPART);
            strcpy(departamentos->departamentos[departamentos->contador].estado, ESTADO_A);

            return departamentos->contador++;
        }
    }

    return -1;
}

/**
 * @brief Esta função atualiza os dados de um departamento
 * 
 * @param departamento
 */
void editarDepartamento(Departamento *departamento) {
    lerString((*departamento).nomeDepart, DEPARTAMENTO_MAX_TAM, OBTER_NOME_DEPART);
    (*departamento).codFuncRespomsavel = obterInt(COD_FUNC_MIN_DEPARTAMENTO, COD_FUNC_MAX_DEPARTAMENTO, OBTER_COD_FUNC_RESPONSAVEL);
    strcpy((*departamento).estado, ESTADO_A);
}

/**
 * @brief Esta função aumenta o tamanho da memória para registar departamentos
 * 
 * @param departamentos
 */
void expandirDepartamentos(Departamentos *departamentos) {
    Departamento *temp = (Departamento*) realloc(departamentos->departamentos, sizeof (Departamento) * (departamentos->tamanho * 2));
    if (temp != NULL) {
        departamentos->tamanho *= 2;
        departamentos->departamentos = temp;
    }
}

/**
 * @brief Verifica se o número do contador e do tamanho são iguais, se sim chama a função expandirDepartamentos()
 * Caso o contador seja menor que o tamanho verifica-se se o registo do departamento correu bem, caso não corresse retorna -1 e aparece uma mensagem (DEPART_EXISTE)
 * Se o valor retornado for diferente de -1 chama-se a função registarDepartFX() para escrever no ficheiro o registo
 * 
 * @param departamentos
 * @param funcionarios
 * @param ficheiro
 */
void registarDepartamentos(Departamentos *departamentos, Funcionarios *funcionarios, char *ficheiro) {
    if (departamentos->contador == departamentos->tamanho) {
        expandirDepartamentos(departamentos);
    }

    if (departamentos->contador < departamentos->tamanho) {
        if (registarDepartamento(departamentos, funcionarios) == -1) {
            puts(DEPART_EXISTE);
        } else {
            registarDepartFX(*departamentos, ficheiro);
        }
    } else {
        puts(DEPART_LISTA_CHEIA);
    }
}

/**
 * @brief Esta função atualiza o estado do departamento para "Inativo"
 * 
 * @param departamento
 */
void removerDepartamento(Departamento *departamento) {
    strcpy((*departamento).estado, ESTADO_D);
}

/**
 * @brief Verifica se o código de departamento inserido pelo utilizador existe
 * O departamento não se elimina, simplesmente muda-se o estado para "Inativo" (removerDepartamento() e editarDepartFX())
 * 
 * 
 * @param departamentos
 * @param carreiras
 * @param ficheiro
 */
void removerDepartamentos(Departamentos *departamentos, Carreiras *carreiras, char *ficheiro) {
    int i, codDepartamento;
    codDepartamento = obterInt(COD_MIN_DEPARTAMENTO, COD_MAX_DEPARTAMENTO, OBTER_COD_DEPART);

    if (procurarDepartamento(*departamentos, codDepartamento) != -1) {
        if (procurarDepartCarreira(*carreiras, codDepartamento) == -1) {
            removerDepartamento(&(*departamentos).departamentos[procurarDepartamento(*departamentos, codDepartamento)]);
            editarDepartFX(*departamentos, procurarDepartamento(*departamentos, codDepartamento), ficheiro);
        } else {
            puts(DEPART_CARREIRA);
        }
    } else {
        puts(DEPART_NAO_EXISTE);
    }
}

/**
 * @brief Verifica-se o código do departamento inserido pelo utilizador existe
 * Se sim chama-se a função editarDepartamento() que vai mudar os dados do funcionário
 * Chama-se também a função editarDepartFX() para editar o registo no ficheiro binário
 * 
 * 
 * @param departamentos
 * @param ficheiro
 */
void editarDepartamentos(Departamentos *departamentos, char *ficheiro) {
    int codDepartamento = procurarDepartamento(*departamentos, obterInt(COD_MIN_DEPARTAMENTO, COD_MAX_DEPARTAMENTO, OBTER_COD_DEPART));

    if (codDepartamento != -1) {
        editarDepartamento(&(*departamentos).departamentos[codDepartamento]);

        editarDepartFX(*departamentos, codDepartamento, ficheiro);
    } else {
        puts(DEPART_NAO_EXISTE);
    }
}

/**
 * @brief Lista os departamentos existentes
 * Caso não haja departamentos aparece uma mensagem a informar que não há (DEPART_LISTA_VAZIA)
 * 
 * @param departamentos
 */
void listarDepartamentos(Departamentos departamentos) {
    int i;

    if (departamentos.contador > 0) {
        printf("--------------- DEPARTAMENTOS ---------------\n");
        for (i = 0; i < departamentos.contador; i++) {
            imprimirDepartamentos(departamentos.departamentos[i]);
        }
    } else {
        puts(DEPART_LISTA_VAZIA);
    }
}

/*
int procurarFuncDeparts(Departamentos departamentos, Funcionarios funcionarios) {
    int i;
    for (i = 0; i < departamentos.contador; i++) {
        if (departamentos.departamentos[i].codDepartamento == funcionarios.funcionarios.) {
            return i;
        }
    }

    return -1;
}
*/

/*
void departFuncAtivos(Departamentos departamentos, Funcionarios funcionarios) {
    int i, j;
    int codDepartamento;

    codDepartamento = obterInt(COD_MIN_DEPARTAMENTO, COD_MAX_DEPARTAMENTO, OBTER_COD_DEPART);

    if (departamentos.contador > 0) {
        if (procurarDepartamento(departamentos, codDepartamento) != -1) {
            printf("--------------- FUNCIONÁRIOS ---------------\n");
            for (i = 0; i < departamentos.contador; i++) {
                for (j = 0; j < funcionarios.contador; j++) {
                    if (departamentos.departamentos[i].cod == funcionarios.funcionarios[j].codFuncionario)

                        imprimirDepartamentos(departamentos.departamentos[i]);
                }
            }
        } else {
            puts(DEPART_LISTA_VAZIA);
        }




    }
}
*/




/**
 * @brief inclusão de interfaces necessárias para o trabalho
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "input.h"

/**
 * @brief Esta função mostra no ecrã os dados existentes do registo dos funcionários
 * 
 * @param funcionario
 */
void imprimirFuncionarios(Funcionario funcionario) {

    printf("\tCódigo: %d\n", funcionario.codFuncionario);
    printf("\tNome: %s\n", funcionario.nome);
    printf("\tNaturalidade: %s\n", funcionario.naturalidade);
    printf("\tNIF: %d\n", funcionario.nif);
    printf("\tNº de Segurança Social: %.0f\n", funcionario.niss);
    printf("\tNº de Utente de Saúde: %d\n", funcionario.numUtenteSaude);
    printf("\tMorada: %s\n", funcionario.morada);
    printf("\tTelefone: %d\n", funcionario.telefone);
    printf("\tE-mail: %s\n", funcionario.mail);
    printf("\tEstado: %s\n", funcionario.estado);
    puts(BARRA);

}

/**
 * @brief Esta função mostra no ecrã o registo dos contactos do funcionário
 * 
 * @param funcionario
 */

void imprimirFuncionarioContact(Funcionario funcionario) {

    printf("\tCodigo: %d\n", funcionario.codFuncionario);
    printf("\tNome: %s\n", funcionario.nome);
    printf("\tTelefone: %d\n", funcionario.telefone);
    printf("\tE-mail: %s\n", funcionario.mail);
    puts(BARRA);

}

/**
 * @brief Esta função "apaga" o registo dos funcionário e colocam as variáveis com números(int, double) a 0 e as variáveis de texto (string) sem texto ("")
 * 
 * @param funcionario
 */

void apagarDadosFuncionarios(Funcionario *funcionario) {
    funcionario->codFuncionario = 0;
    funcionario->nif = 0;
    funcionario->niss = 0;
    funcionario->numUtenteSaude = 0;
    funcionario->telefone = 0;
    strcpy(funcionario->nome, "");
    strcpy(funcionario->morada, "");
    strcpy(funcionario->naturalidade, "");
    strcpy(funcionario->estado, "");
    strcpy(funcionario->mail, "");
}

/**
 * @brief Esta função verifica se o funcionário existe nos registos dos Funcionários
 * 
 * @param funcionarios
 * @param codFunc
 * @return i se o funcionário existir e -1 se ele não existir
 */

int procurarFuncionario(Funcionarios funcionarios, int codFunc) {
    int i;
    for (i = 0; i < funcionarios.contador; i++) {
        if (funcionarios.funcionarios[i].codFuncionario == codFunc) {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Esta função verifica se o ficheiro existe(rb), senão cria-o (wb)
 * Com a função fread é retornado o número de itens completos lidos pela função
 * 
 * @param funcionarios
 * @param ficheiro
 */
void carregarFuncionarios(Funcionarios *funcionarios, char *ficheiro) {
    int i, sucesso = 0;

    //Tenta abrir o ficheiro
    FILE *fp = fopen(ficheiro, "rb");
    if (fp != NULL) {

        //retorna o número de itens completos lidos pela função
        fread(&funcionarios->contador, sizeof (int), 1, fp);

        if (funcionarios->contador > 0) {
            funcionarios->funcionarios = (Funcionario*) malloc(funcionarios->contador * sizeof (Funcionario));
            funcionarios->tamanho = funcionarios->contador;
            fread(funcionarios->funcionarios, sizeof (Funcionario), funcionarios->contador, fp);

            sucesso = 1;
        }
        fclose(fp);
    }

    if (!sucesso) {
        fp = fopen(ficheiro, "wb");
        if (fp != NULL) {

            funcionarios->funcionarios = (Funcionario*) malloc(FUNCIONARIO_QUANT_INICIAL * sizeof (Funcionario));
            funcionarios->contador = 0;
            funcionarios->tamanho = FUNCIONARIO_QUANT_INICIAL;
            fclose(fp);
        }
    }

}

/**
 * @brief Esta função desaloca a memória alocada 
 * 
 * @param funcionarios
 */
void libertarFuncionarios(Funcionarios *funcionarios) {
    free(funcionarios->funcionarios);

}

/**
 * @brief Esta função guarda o contador dos registos
 * 
 * @param contador
 * @param ficheiro
 */
void atualizarFuncContadorFX(int contador, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fwrite(&contador, sizeof (int), 1, fp);
    fclose(fp);
}

/**
 * @brief Esta função regista o funcionário no ficheiro binário
 * 
 * @param funcionarios
 * @param ficheiro
 */
void registarFuncFX(Funcionarios funcionarios, char *ficheiro) {
    atualizarFuncContadorFX(funcionarios.contador, ficheiro);

    int i;
    FILE *fp = fopen(ficheiro, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&funcionarios.funcionarios[funcionarios.contador - 1], sizeof (Funcionario), 1, fp);
    fclose(fp);
}

/**
 * @brief Esta função atualiza os dados de um funcionário no ficheiro binário
 * 
 * @param funcionarios
 * @param indice
 * @param ficheiro
 */
void editarFuncFX(Funcionarios funcionarios, int indice, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "rb+");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof (int), SEEK_SET);

    fseek(fp, sizeof (Funcionario) * indice, SEEK_CUR);

    fwrite(&funcionarios.funcionarios[indice], sizeof (Funcionario), 1, fp);
}

/**
 * @brief Esta função remove o registo do funcionário no ficheiro binário
 * 
 * @param funcionarios
 * @param ficheiro
 */
void removerFuncFX(Funcionarios funcionarios, char *ficheiro) {
    FILE *fp = fopen(ficheiro, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fwrite(&funcionarios.contador, sizeof (int), 1, fp);
    fwrite(funcionarios.funcionarios, sizeof (Funcionario), funcionarios.contador, fp);
}

/**
 * @brief Verifica se o código do funcionário que o utilizador inseriu existe
 * Insere um novo registo em Funcionarios e o contador aumento por 1
 * 
 * 
 * @param funcionarios
 * @return -1 se o código já existir ou retorna o número do contador se os dados foram registados com sucesso
 */
int registarFuncionario(Funcionarios *funcionarios) {
    int codFuncionario;
    codFuncionario = obterInt(COD_MIN_FUNCIONARIO, COD_MAX_FUNCIONARIO, OBTER_COD_FUNCIONARIO);


    if (procurarFuncionario(*funcionarios, codFuncionario) == -1) {

        funcionarios->funcionarios[funcionarios->contador].codFuncionario = codFuncionario;
        lerString(funcionarios->funcionarios[funcionarios->contador].nome, FUNCIONARIO_MAX_TAM, OBTER_NOME_FUNCIONARIO);
        lerString(funcionarios->funcionarios[funcionarios->contador].morada, FUNCIONARIO_MAX_TAM, OBTER_MORADA_FUNCIONARIO);
        lerString(funcionarios->funcionarios[funcionarios->contador].naturalidade, FUNCIONARIO_MAX_TAM, OBTER_NATURALIDADE_FUNC);
        funcionarios->funcionarios[funcionarios->contador].telefone = obterInt(TELE_MIN_FUNCIONARIO, TELE_MAX_FUNCIONARIO, OBTER_TELE_FUNCIONARIO);
        lerString(funcionarios->funcionarios[funcionarios->contador].mail, FUNCIONARIO_MAX_TAM, OBTER_EMAIL_FUNCIONARIO);
        funcionarios->funcionarios[funcionarios->contador].nif = obterInt(NIF_MIN_FUNCIONARIO, NIF_MAX_FUNCIONARIO, OBTER_NIF_FUNCIONARIO);
        funcionarios->funcionarios[funcionarios->contador].niss = obterDouble(NISS_MIN_FUNCIONARIO, NISS_MAX_FUNCIONARIO, OBTER_NISS_FUNCIONARIO);
        funcionarios->funcionarios[funcionarios->contador].numUtenteSaude = obterInt(NUM_UTENTE_SAUDE_MIN_FUNCIONARIO, NUM_UTENTE_SAUDE_MAX_FUNCIONARIO, OBTER_NUM_UTENTE_SAUDE_FUNCIONARIO);
        strcpy(funcionarios->funcionarios[funcionarios->contador].estado, ESTADO_A);

        return funcionarios->contador++;
    }

    return -1;
}

/**
 * @brief Esta função atualiza os dados de um funcionário
 * 
 * @param funcionario
 */
void editarFuncionario(Funcionario *funcionario) {

    lerString((*funcionario).nome, FUNCIONARIO_MAX_TAM, OBTER_NOME_FUNCIONARIO);
    lerString((*funcionario).naturalidade, FUNCIONARIO_MAX_TAM, "NATU ");
    lerString((*funcionario).morada, FUNCIONARIO_MAX_TAM, OBTER_MORADA_FUNCIONARIO);
    lerString((*funcionario).mail, FUNCIONARIO_MAX_TAM, OBTER_EMAIL_FUNCIONARIO);
    (*funcionario).telefone = obterInt(TELE_MIN_FUNCIONARIO, TELE_MAX_FUNCIONARIO, OBTER_TELE_FUNCIONARIO);
    (*funcionario).nif = obterInt(NIF_MIN_FUNCIONARIO, NIF_MAX_FUNCIONARIO, OBTER_NIF_FUNCIONARIO);
    (*funcionario).niss = obterDouble(NISS_MIN_FUNCIONARIO, NISS_MAX_FUNCIONARIO, OBTER_NISS_FUNCIONARIO);
    (*funcionario).numUtenteSaude = obterInt(NUM_UTENTE_SAUDE_MIN_FUNCIONARIO, NUM_UTENTE_SAUDE_MAX_FUNCIONARIO, OBTER_NUM_UTENTE_SAUDE_FUNCIONARIO);
    strcpy((*funcionario).estado, ESTADO_A);
}

/**
 * @brief Esta função aumenta o tamanho da memória para registar funcionários
 * 
 * @param funcionarios
 */
void expandirFuncionarios(Funcionarios *funcionarios) {
    Funcionario *temp = (Funcionario*) realloc(funcionarios->funcionarios, sizeof (Funcionario) * (funcionarios->tamanho * 2));
    if (temp != NULL) {
        funcionarios->tamanho *= 2;
        funcionarios->funcionarios = temp;
    }
}

/**
 * @brief Verifica se o número do contador e do tamanho são iguais, se sim chama a função expandirFuncionarios()
 * Caso o contador seja menor que o tamanho verifica-se se o registo do funcionário correu bem, caso não corresse retorna -1 e aparece uma mensagem (FUNC_EXISTE)
 * Se o valor retornado for diferente de -1 chama-se a função registarFuncFX() para escrever no ficheiro o registo
 * 
 * @param funcionarios
 * @param ficheiro
 */
void registarFuncionarios(Funcionarios *funcionarios, char *ficheiro) {
    if (funcionarios->contador == funcionarios->tamanho) {
        expandirFuncionarios(funcionarios);
    }

    if (funcionarios->contador < funcionarios->tamanho) {
        if (registarFuncionario(funcionarios) == -1) {
            puts(FUNC_EXISTE);
        } else {
            registarFuncFX(*funcionarios, ficheiro);
            puts(FUNCIONARIO_REGISTADO_SUCESSO);
        }
    } else {
        puts(FUNC_LISTA_CHEIA);
    }
}

/**
 * @brief Esta função atualiza o estado do funcionário para "Inativo"
 * 
 * @param funcionario
 */
void removerFuncionario(Funcionario *funcionario) {
    strcpy((*funcionario).estado, ESTADO_D);
}

/**
 * @brief Verifica se o código de funcionário inserido pelo utilizador existe
 * Se o funcionário estiver registado num contrato o seu estado muda para "Inativo" (removerFuncionario() e editarFuncFX())
 * Caso contrário o registo do funcionário é removido (removerFuncFX()) e o contador descresce por 1
 * 
 * @param funcionarios
 * @param carreiras
 * @param ficheiro
 */
void removerFuncionarios(Funcionarios *funcionarios, Carreiras *carreiras, char *ficheiro) {
    int i, codFuncionario, numero;

    codFuncionario = obterInt(COD_MIN_FUNCIONARIO, COD_MAX_FUNCIONARIO, OBTER_COD_FUNCIONARIO);
    numero = procurarFuncionario(*funcionarios, codFuncionario);

    if (numero != -1) {
        if (procurarFuncCarreira(*carreiras, codFuncionario) == -1) {
            for (i = numero; i < funcionarios->contador - 1; i++) {
                funcionarios->funcionarios[i] = funcionarios->funcionarios[i + 1];
            }

            apagarDadosFuncionarios(&funcionarios->funcionarios[i]);
            funcionarios->contador--;
            removerFuncFX(*funcionarios, ficheiro);
            puts(FUNCIONARIO_REMOVIDO_SUCESSO);
        } else {
            removerFuncionario(&(*funcionarios).funcionarios[numero]);
            editarFuncFX(*funcionarios, numero, ficheiro);
        }
    } else {
        puts(FUNC_NAO_EXISTE);
    }
}

/**
 * @brief Verifica-se o código do funcionário inserido pelo utilizador existe
 * Se sim chama-se a função editarFuncionario() que vai mudar os dados do funcionário
 * Chama-se também a função editarFuncFX() para editar o registo no ficheiro binário
 * 
 * @param funcionarios
 * @param ficheiro
 */
void editarFuncionarios(Funcionarios *funcionarios, char *ficheiro) {
    int codFuncionario = procurarFuncionario(*funcionarios, obterInt(COD_MIN_FUNCIONARIO, COD_MAX_FUNCIONARIO, OBTER_COD_FUNCIONARIO));

    if (codFuncionario != -1) {
        editarFuncionario(&(*funcionarios).funcionarios[codFuncionario]);

        editarFuncFX(*funcionarios, codFuncionario, ficheiro);
        puts(FUNCIONARIO_EDITADO_SUCESSO);
    } else {
        puts(FUNC_NAO_EXISTE);
    }
}

/**
 * @brief Esta função lista os contacto dos funcionários(e-mail, telemóvel)
 * 
 * @param funcionarios
 */
void listarFuncionariosContactos(Funcionarios funcionarios) {
    int i;

    if (funcionarios.contador > 0) {
        printf("\n--------------- FUNCIONÁRIOS ---------------\n\n");
        for (i = 0; i < funcionarios.contador; i++) {
            imprimirFuncionarioContact(funcionarios.funcionarios[i]);
        }
    } else {
        puts(FUNC_LISTA_VAZIA);
    }
}

/**
 * @brief Esta função lista os funcionário existentes
 * 
 * @param funcionarios
 */
void listarFuncionarios(Funcionarios funcionarios) {
    int i;

    if (funcionarios.contador > 0) {
        printf("\n--------------- FUNCIONÁRIOS ---------------\n\n");
        for (i = 0; i < funcionarios.contador; i++) {
            imprimirFuncionarios(funcionarios.funcionarios[i]);
        }
    } else {
        puts(FUNC_LISTA_VAZIA);
    }
}





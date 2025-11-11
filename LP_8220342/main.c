/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * @file   main.c
 * @author Daniela Mendes
 * @date 6 de Fevereiro 2023
 * 
 * Ficheiro que mostra os menus que o utilizador pode escolher para fazer a gestão da empresa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Bibliotecas utilizadas
 */

#include "funcionalidades.h"
#include "input.h"

/**
 * @brief Ficheiros utilizados para guardar os registos
 */
#define FUNC_DB_FILE "funcionarios.bin"
#define DEPART_DB_FILE "departamentos.bin"
#define CARREIRAS_DB_FILE "carreiras.bin"
#define CARREIRAS1_TXT_FILE "contratosAtuais.txt"
#define CARREIRAS2_TXT_FILE "contratosAntigos.txt"

/**
 * @brief Função com as funções da gestão dos funcionários(criar, eliminar, editar, ...) 
 */
void gestaoFuncionarios() {
    int opcao;
    Funcionarios funcionarios;
    Carreiras carreiras;

    carregarFuncionarios(&funcionarios, FUNC_DB_FILE);
    do {


        printf("-------- GESTÃO DE FUNCIONÁRIOS --------\n");
        printf("\t1 - Inserir Funcionários\n");
        printf("\t2 - Remover Funcionários\n");
        printf("\t3 - Editar Funcionários\n");
        printf("\t4 - Listagem de Contactos de Funcionários\n");
        printf("\t5 - Listagem de Funcionários\n");
        printf("\t0 - Sair\n");
        printf("\nFuncionários: %d-%d", funcionarios.contador, funcionarios.tamanho);

        opcao = obterInt(0, 5, "\nOpção: ");

        switch (opcao) {
            case 0:
                puts("");
                break;
            case 1:
                registarFuncionarios(&funcionarios, FUNC_DB_FILE);
                puts("");
                break;
            case 2:
                carregarCarreiras(&carreiras, CARREIRAS_DB_FILE);
                removerFuncionarios(&funcionarios, &carreiras, FUNC_DB_FILE);
                puts("");
                break;
            case 3:
                editarFuncionarios(&funcionarios, FUNC_DB_FILE);
                puts("");
                break;
            case 4:
                listarFuncionariosContactos(funcionarios);
                break;
            case 5:
                listarFuncionarios(funcionarios);
                break;
            default:
                printf("\nOpcão invalida!");
                puts("");
                break;
        }

    } while (opcao != 0);

    libertarFuncionarios(&funcionarios);
}

/**
 * @brief Função com as funções da gestão dos departamentos (criar, eliminar, editar, ...) 
 */

void gestaoDepartamentos() {
    int opcao;
    Departamentos departamentos = {.contador = 0};
    Funcionarios funcionarios;
    Carreiras carreiras;

    carregarDepartamentos(&departamentos, DEPART_DB_FILE);

    do {


        printf("-------- GESTÃO DE DEPARTAMENTOS --------\n");
        printf("\t1 - Inserir Departamento\n");
        printf("\t2 - Remover Departamento\n");
        printf("\t3 - Editar Departamento\n");
        printf("\t4 - Listagem de Departamento\n");
        printf("\t0 - Sair\n");
        printf("\nDepartamentos: %d-%d", departamentos.contador, departamentos.tamanho);

        opcao = obterInt(0, 4, "\nOpção: ");

        switch (opcao) {
            case 0:
                puts("");
                break;
            case 1:
                carregarFuncionarios(&funcionarios, FUNC_DB_FILE);
                registarDepartamentos(&departamentos, &funcionarios, DEPART_DB_FILE);
                puts("");
                break;
            case 2:
                carregarCarreiras(&carreiras, CARREIRAS_DB_FILE);
                carregarDepartamentos(&departamentos, DEPART_DB_FILE);
                removerDepartamentos(&departamentos, &carreiras, DEPART_DB_FILE);
                puts("");
                break;
            case 3:
                editarDepartamentos(&departamentos, DEPART_DB_FILE);
                puts("");
                break;
            case 4:
                listarDepartamentos(departamentos);
                break;
            default:
                printf("\nOpcão invalida!");
                puts("");
                break;
        }

    } while (opcao != 0);

    libertarDepartamentos(&departamentos);

}

/**
 * @brief Função com as funções da gestão dos funcionários(criar, listar e exportar) 
 */

void gestaoCarreiras() {
    int opcao;
    int codDepart;
    Departamentos departamentos;
    Funcionarios funcionarios;
    Carreiras carreiras;

    carregarCarreiras(&carreiras, CARREIRAS_DB_FILE);

    do {
        printf("-------- GESTÃO DE CARREIRAS --------\n");
        printf("\t1 - Inserir Carreiras\n");
        printf("\t2 - Listagem de Funcionários por Departamento\n");
        printf("\t3 - Listagem de Contratos a Terminar\n");
        printf("\t4 - Exportar Contratos Ativos \n");
        printf("\t5 - Exportar Contratos Inativos\n");
        printf("\t6 - Listagem de Contratos\n");
        printf("\t7 - Listagem Funcionários Ativos\n");
        printf("\t0 - Sair\n");
        printf("\nCarreiras: %d-%d", carreiras.contador, carreiras.tamanho);

        opcao = obterInt(0, 9, "\nOpção: ");

        switch (opcao) {
            case 0:
                puts("");
                break;
            case 1:
                carregarFuncionarios(&funcionarios, FUNC_DB_FILE);
                carregarDepartamentos(&departamentos, DEPART_DB_FILE);
                registarCarreiras(&carreiras, &funcionarios, &departamentos, CARREIRAS_DB_FILE);
                puts("");
                break;
            case 2:
                listarFuncionariosCarreiras(carreiras);
                break;
            case 3:
                listarContratosCarreiras(carreiras);
                break;
            case 4:
                if (exportarAtualDF(carreiras, CARREIRAS_DB_FILE, CARREIRAS1_TXT_FILE) == 1)
                    printf("\n-----FICHEIRO EXPORTADO COM SUCESSO -----\n");
                else
                    printf("\n-----FICHEIRO EXPORTADO SEM SUCESSO -----\n");
                break;
            case 5:
                if (exportarOldDF(carreiras, CARREIRAS_DB_FILE, CARREIRAS2_TXT_FILE) == 1)
                    printf("\n-----FICHEIRO EXPORTADO COM SUCESSO -----\n");
                else
                    printf("\n-----FICHEIRO EXPORTADO SEM SUCESSO -----\n");
                break;
            case 6:
                listarCarreiras(carreiras);
                break;
            case 7:
                puts(OBTER_COD_DEPART);
                scanf("%d", &codDepart);
                carregarFuncionarios(&funcionarios, FUNC_DB_FILE);
                departFuncAtivos(carreiras, funcionarios, codDepart);
            default:
                printf("\n----- OPÇÃO INVÁLIDA -----\n\n");
                break;
        }

    } while (opcao != 0);

    libertarCarreiras(&carreiras);
}

/**
 * @brief Esta função chama as funções acima dependendo do número da função que o utilizador escolher
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    int opcao;

    do {

        printf("----------- MÓVEIS PARA TODOS -----------\n");
        printf("\t1 - Gestão de Funcionários\n");
        printf("\t2 - Gestão de Departamento\n");
        printf("\t3 - Gestão de Carreiras\n");
        printf("\t0 - Sair\n");
        puts(BARRA);

        opcao = obterInt(0, 3, "\nOpção: ");

        switch (opcao) {

            case 0:
                puts("");
                break;
            case 1:
                gestaoFuncionarios();
                puts("");
                break;
            case 2:
                gestaoDepartamentos();
                puts("");
                break;
            case 3:
                gestaoCarreiras();
                puts("");
                break;
        }

    } while (opcao != 0);

    return (EXIT_SUCCESS);
}


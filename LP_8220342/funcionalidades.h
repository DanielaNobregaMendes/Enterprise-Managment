#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H



/**
 * @brief Macros usadas no que diz respeito aos funcionários 
 */

#define FUNC_NAO_EXISTE "\n------ FUNCIONÁRIO INEXISTENTE ------\n\n"
#define FUNC_EXISTE "\n------ FUNCIONÁRIO JÁ EXISTENTE ------\n\n"
#define FUNC_LISTA_CHEIA "\n------ NÃO É POSSÍVEL INSERIR OUTRO FUNCIONÁRIO ------\n\n"
#define FUNC_LISTA_VAZIA "\n------ LISTA DE FUNCIONÁRIOS VAZIA ------\n\n"
#define ESTADO_D "Inativo"
#define ESTADO_A "Ativo"

#define COD_MAX_FUNCIONARIO 99999 
#define COD_MIN_FUNCIONARIO 10000 
#define OBTER_COD_FUNCIONARIO "Insira o código do funcionário: "


#define NIF_MAX_FUNCIONARIO 999999999 
#define NIF_MIN_FUNCIONARIO 100000000
#define OBTER_NIF_FUNCIONARIO "Insira o NIF do funcionário: "

#define NUM_UTENTE_SAUDE_MAX_FUNCIONARIO 999999999 
#define NUM_UTENTE_SAUDE_MIN_FUNCIONARIO 100000000
#define OBTER_NUM_UTENTE_SAUDE_FUNCIONARIO "Insira o Nº de Utente de Saúde do funcionário: "

#define NISS_MAX_FUNCIONARIO 99999999999 
#define NISS_MIN_FUNCIONARIO 10000000000 
#define OBTER_NISS_FUNCIONARIO "Insira o Nº de Segurança Social do funcionário: "


#define TELE_MAX_FUNCIONARIO 999999999 
#define TELE_MIN_FUNCIONARIO 100000000 
#define OBTER_TELE_FUNCIONARIO "Insira o nº de telemóvel do funcionário: "

#define FUNCIONARIO_MAX_TAM 100
#define OBTER_NOME_FUNCIONARIO "Insira o nome do funcionário: "
#define OBTER_MORADA_FUNCIONARIO "Insira a morada do funcionário: "
#define OBTER_NATURALIDADE_FUNC "Insira a naturalidade do funcionário: "
#define OBTER_EMAIL_FUNCIONARIO "Insira o email do funcionário: "

#define FUNCIONARIO_REGISTADO_SUCESSO "\n------ FUNCIONÁRIO REGISTADO COM SUCESSO ------\n\n"
#define FUNCIONARIO_EDITADO_SUCESSO "\n------ FUNCIONÁRIO ATUALIZADO COM SUCESSO ------\n\n"
#define FUNCIONARIO_REMOVIDO_SUCESSO "\n------ FUNCIONÁRIO REMOVIDO COM SUCESSO ------\n\n"



#define FUNCIONARIO_ESTADO_MAX_TAM 8

#define FUNCIONARIO_QUANT_INICIAL 1
#define COD_MAX_FUNCIONARIO 99999 

#define BARRA "----------------------------------------\n"

/**
 * @brief Macros usadas no que diz respeito aos departamentos
 */

#define DEPART_NAO_EXISTE "\n------ DEPARTAMENTO INEXISTENTE ------\n\n"
#define DEPART_EXISTE "\n------ DEPARTAMENTO JÁ EXISTENTE ------\n\n"
#define DEPART_LISTA_CHEIA "\n------ NÃO É POSSÍVEL INSERIR OUTRO DEPARTAMENTO ------\n\n"
#define DEPART_LISTA_VAZIA "\n------ LISTA DE DEPARTAMENTOS VAZIA ------\n\n"
#define DEPART_CARREIRA "\n------ DEPARTAMENTO COM FUNCIONÁRIOS ------\n\n"


#define COD_MAX_DEPARTAMENTO 9999 
#define COD_MIN_DEPARTAMENTO 1000 
#define OBTER_COD_DEPART "Insira o código do departamento: "

#define DEPARTAMENTO_MAX_TAM 80
#define OBTER_NOME_DEPART "Insira o nome do departamento: "

#define COD_FUNC_MAX_DEPARTAMENTO 99999 
#define COD_FUNC_MIN_DEPARTAMENTO 10000 
#define OBTER_COD_FUNC_RESPONSAVEL "Insira o código do funcionário responsável pelo departamento: "


#define DEPARTAMENTO_QUANT_INICIAL 1

#define DEPARTAMENTO_ESTADO_MAX_TAM 20

/**
 * @brief Macros usadas no que diz respeito aos contratos
 */

#define CARREIRAS_NAO_EXISTE "----- CONTRATO INEXISTENTE -----"
#define CARREIRAS_EXISTE "----- CONTRATO JÁ EXISTENTE -----"
#define CARREIRAS_LISTA_CHEIA "\n------ NÃO É POSSÍVEL INSERIR OUTRO CONTRATO ------\n\n"
#define CARREIRAS_LISTA_VAZIA "----- LISTA DE CONTRATOS VAZIA -----"
#define CARREIRAS_DEPART "----- FUNCIONÁRIO NÃO ATRIBUIDO A NENHUM DEPARTAMENTO -----"
#define OBTER_NOME_CARREIRAS "Insira o nome do departamento: "
#define OBTER_COD_FUNC_RESPONSAVEL "Insira o código do funcionário responsável pelo departamento: "
#define MESES_CARREIRAS_MIN 1
#define MESES_CARREIRAS_MAX 60

#define CARREIRAS_QUANT_INICIAL 1


/**
 * @brief A estrutura de dado permitem guardar dados do funcionário
 */
typedef struct {
    int codFuncionario;
    int nif;
    int telefone;
    int numUtenteSaude;
    double niss;
    char naturalidade[FUNCIONARIO_MAX_TAM];
    char estado[FUNCIONARIO_ESTADO_MAX_TAM];
    char mail[FUNCIONARIO_MAX_TAM];
    char nome[FUNCIONARIO_MAX_TAM];
    char morada[FUNCIONARIO_MAX_TAM];
} Funcionario;

typedef struct {
    int contador, tamanho;
    Funcionario *funcionarios;
} Funcionarios;

/**
 * @brief A estrutura de dado permitem guardar dados dos departamentos
 */
typedef struct {
    int codDepartamento;
    int codFuncRespomsavel;
    char nomeDepart[DEPARTAMENTO_MAX_TAM];
    char estado[DEPARTAMENTO_ESTADO_MAX_TAM];
} Departamento;

typedef struct {
    int contador, tamanho;
    Departamento *departamentos;
} Departamentos;


/**
 * @brief As estruturas de dados permitem guardar dados dos contratos e da data, neste case a data de inicio e de fim
 */
typedef struct {
    int ano_inicio, mes_inicio, dia_inicio;
    int ano_fim, mes_fim, dia_fim;
} Data;

typedef struct {
    int codFuncionario;
    int codDepartamento;
    Data data_carreira;    
} Carreira;

typedef struct {
    int contador, tamanho;
    Carreira *carreiras;
} Carreiras;



/**
 * @brief Funções usadas para os departamentos
 */
void carregarDepartamentos(Departamentos *departamentos, char *ficheiro);
void registarDepartamentos(Departamentos *departamentos, Funcionarios *funcionarios, char *ficheiro);
void procurarDepartamentos(Departamentos departamentos);
int procurarDepartamento(Departamentos departamentos, int codDepartamento);
void removerDepartamentos(Departamentos *departamentos, Carreiras *carreiras, char *ficheiro);
void listarDepartamentos(Departamentos departamentos);
void libertarDepartamentos(Departamentos *departamentos);
void editarDepartamentos(Departamentos *departamentos, char *ficheiro);

/**
 * @brief Funções usadas para os funcionário
 */
void carregarFuncionarios(Funcionarios *funcionarios, char *ficheiro);
void registarFuncionarios(Funcionarios *funcionarios, char *ficheiro);
void produrarFuncionarios(Funcionarios funcionarios);
int procurarFuncionario(Funcionarios funcionarios, int codFunc);
void removerFuncionarios(Funcionarios *funcionarios, Carreiras *carreiras,char *ficheiro);
void listarFuncionarios(Funcionarios funcionarios);
void libertarFuncionarios(Funcionarios *funcionarios);
void editarFuncionarios(Funcionarios *funcionarios, char *ficheiro);
void listarFuncionariosContactos(Funcionarios funcionarios);

/**
 * @brief Funções usadas para os contratos
 */
void carregarCarreiras(Carreiras *carreiras, char *ficheiro);
void registarCarreiras(Carreiras *carreiras, Funcionarios *funcionarios, Departamentos *departamentos,char *ficheiro);
void procurarCarreiras(Carreiras carreiras);
int procurarDepartCarreira(Carreiras carreiras, int codDepartamento);
int procurarFuncCarreira(Carreiras carreiras, int codFuncionario);
void listarCarreiras(Carreiras carreiras);
void libertarCarreiras(Carreiras *carreiras);
int exportarAtualDF(Carreiras carreiras, char *ficheiroBin, char *ficheiroTxt);
int exportarOldDF(Carreiras carreiras, char* ficheiroBin, char* ficheiroTxt);
void listarFuncionariosCarreiras(Carreiras carreiras);
void listarContratosCarreiras(Carreiras carreiras);
void departFuncAtivos(Carreiras carreiras, Funcionarios funcionarios, int codDepartamento);

#endif /* FUNCIONALIDADES_H */


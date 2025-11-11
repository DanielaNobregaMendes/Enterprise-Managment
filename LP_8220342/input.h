/**
 * Esta biblioteca serve para verificar as variáveis introduzidas pelo utilizador
 * 
 * @param minValor
 * @param maxValor
 * @param msg
 * @return 
 */

#ifndef INPUT_H
#define INPUT_H

int obterInt(int minValor, int maxValor, char *msg);

long int obterLongLong(long int minValor, long int maxValor, char *msg);

float obterFloat(float minValor, float maxValor, char *msg);

double obterDouble(double minValor, double maxValor, char *msg);

char obterChar(char *msg);

void lerString(char *string, unsigned int tamanho, char *msg);

#endif /* INPUT_H */


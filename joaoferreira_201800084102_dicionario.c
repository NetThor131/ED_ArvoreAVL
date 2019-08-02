#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char sinonimo[31];
} Sinonimo;

typedef struct
{
    char palavra[31];
    int len;
    Sinonimo *sinonimos;
} Palavra;

typedef struct no
{
    Palavra chave;
    struct no *dir;
    struct no *esq;
    int balanco;
} NO;

void caso1(NO **raiz, int *alterou)
{
    NO *u;
    NO *v;
    u = malloc(sizeof(NO));
    u = (*raiz)->esq;

    if (u->balanco == -1)
    {
        (*raiz)->esq = u->dir;
        u->dir = *raiz;
        (*raiz)->balanco = 0;
        (*raiz) = u;
    }
    else
    {
        v = u->dir;
        u->dir = v->esq;
        v->esq = u;
        (*raiz)->esq = v->dir;
        v->dir = (*raiz);
        if (v->balanco == -1)
            (*raiz)->balanco = 1;
        else
            (*raiz)->balanco = 0;
        if (v->balanco == 1)
            u->balanco = -1;
        else
            u->balanco = 0;
        (*raiz) = v;
    }
    (*raiz)->balanco = 0;
    (*alterou) = 0;
}

void caso2(NO **raiz, int *alterou)
{
    NO *u;
    NO *v;
    u = malloc(sizeof(NO));
    u = (*raiz)->dir;

    if (u->balanco == 1)
    {
        (*raiz)->dir = u->esq;
        u->esq = *raiz;
        (*raiz)->balanco = 0;
        (*raiz) = u;
    }
    else
    {
        v = u->esq;
        u->esq = v->dir;
        v->dir = u;
        (*raiz)->dir = v->esq;
        v->esq = (*raiz);
        if (v->balanco == 1)
            (*raiz)->balanco = -1;
        else
            (*raiz)->balanco = 0;
        if (v->balanco == -1)
            u->balanco = 1;
        else
            u->balanco = 0;
        (*raiz) = v;
    }
    (*raiz)->balanco = 0;
    *alterou = 0;
}

void insereNumaAVL(Palavra x, NO **raiz, int *alterou)
{
    if (*raiz == NULL)
    {
        *raiz = (NO *)malloc(sizeof(NO));
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
        (*raiz)->chave = x;
        (*raiz)->balanco = 0;
        *alterou = 1;
    }
    else
    {
        if (strcmp(x.palavra, (*raiz)->chave.palavra) < 0)
        {
            insereNumaAVL(x, &((*raiz)->esq), alterou);
            if (*alterou)
            {
                if ((*raiz)->balanco == 1)
                {
                    (*raiz)->balanco = 0;
                    alterou = 0;
                }
                else
                {
                    if ((*raiz)->balanco == 0)
                        (*raiz)->balanco = -1;
                    else if ((*raiz)->balanco == -1)
                        caso1(raiz, alterou);
                }
            }
        }
        else if (strcmp(x.palavra, (*raiz)->chave.palavra) > 0)
        {
            insereNumaAVL(x, &((*raiz)->dir), alterou);
            if (*alterou)
            {
                if ((*raiz)->balanco == -1)
                {
                    alterou = 0;
                    (*raiz)->balanco = 0;
                }
                else
                {
                    if ((*raiz)->balanco == 0)
                        (*raiz)->balanco = 1;
                    else if ((*raiz)->balanco == 1)
                        caso2(raiz, alterou);
                }
            }
        }
    }
}

int consulta(NO **raiz, char valor[31], FILE **output)
{
    if (raiz == NULL)
        return 0;
    NO *atual;
    atual = malloc(sizeof(NO));
    atual = (*raiz);
    fprintf(*output, "[");
    while (atual != NULL)
    {
        if (strcmp(valor, atual->chave.palavra) == 0)
        {
            fprintf(*output, "%s]\n", valor);

            fprintf(*output, "%s", atual->chave.sinonimos[0].sinonimo);
            for (int i = 1; i < atual->chave.len; i++)
            {
                fprintf(*output, ",%s", atual->chave.sinonimos[i].sinonimo);
            }
            fprintf(*output, "\n");
            return 1;
        }
        if (strcmp(valor, atual->chave.palavra) > 0)
        {
            fprintf(*output, "%s->", atual->chave.palavra);
            atual = atual->dir;
        }
        else if (strcmp(valor, atual->chave.palavra) < 0)
        {
            fprintf(*output, "%s->", atual->chave.palavra);
            atual = atual->esq;
        }
    }
    fprintf(*output, "?]\n-\n");
    return 0;
}

int main(int argc, char *argv[])
{
    printf("inicio do main\n");
    int h;
    printf("1\n");
    NO *raiz = NULL;
    printf("criei a arvore\n");

    FILE *input = fopen(argv[1], "r");
    printf("input\n");
    FILE *output = fopen(argv[2], "w");
    printf("output\n");

    int qntd = 0, qntdsin = 0;
    printf("qntd e qntdsin\n");
    char palavra[31], sinonimo[31];
    printf("palavra e sinonimo\n");

    fscanf(input, "%i", &qntd);
    printf("qntd = %i\n", qntd);

    Palavra *palavras;
    printf("vetor de palavras\n");
    palavras = (Palavra *)malloc(qntd * sizeof(Palavra));
    printf("malloc no vetor de palavras\n");

    for (int i = 0; i < qntd; i++)
    {
        printf("primeiro for\n");
        fscanf(input, "%s %i", palavra, &qntdsin);
        printf("palavra = %s e qntd de sinonimos = %i\n", palavra, qntdsin);
        strcpy(palavras[i].palavra, palavra);
        printf("palavras[i].palavra = %s\n", palavras[i].palavra);
        palavras[i].len = qntdsin;
        printf("palavras[i].len = %i\n", palavras[i].len);
        palavras[i].sinonimos = (Sinonimo *)malloc(qntdsin * sizeof(Sinonimo));
        printf("malloc no sinonimos\n");

        for (int x = 0; x < qntdsin; x++)
        {
            printf("for dentro do primeiro for\n");
            fscanf(input, "%s", sinonimo);
            printf("sinonimo = %s\n", sinonimo);
            strcpy(palavras[i].sinonimos[x].sinonimo, sinonimo);
            printf("palavras[i].sinonimos[x].sinonimo = %s\n", palavras[i].sinonimos[x].sinonimo);
        }
        insereNumaAVL(palavras[i], &raiz, &h);
        printf("insercao\n");
    }

    fscanf(input, "%i", &qntd);
    printf("qntd = %i\n", qntd);
    for (int i = 0; i < qntd; i++)
    {
        printf("segundo for\n");
        fscanf(input, "%s", palavra);
        printf("palavra = %s", palavra);
        consulta(&raiz, palavra, &output);
        printf("consulta\n");
    }

    fclose(input);
    fclose(output);

    return 0;
}
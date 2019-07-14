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

typedef struct node
{
    Palavra chave;
    struct node *filhoDir;
    struct node *filhoEsq;
    int balanco;
} noh;

void caso1(noh **raiz, int *alterou)
{
    noh *u;
    noh *v;
    u = malloc(sizeof(noh));
    u = (*raiz)->filhoEsq;

    if (u->balanco == -1)
    {
        (*raiz)->filhoEsq = u->filhoDir;
        u->filhoDir = *raiz;
        (*raiz)->balanco = 0;
        (*raiz) = u;
    }
    else
    {
        v = u->filhoDir;
        u->filhoDir = v->filhoEsq;
        v->filhoEsq = u;
        (*raiz)->filhoEsq = v->filhoDir;
        v->filhoDir = (*raiz);
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

void caso2(noh **raiz, int *alterou)
{
    noh *u;
    noh *v;

    u = malloc(sizeof(noh));
    u = (*raiz)->filhoDir;

    if (u->balanco == 1)
    {
        (*raiz)->filhoDir = u->filhoEsq;
        u->filhoEsq = *raiz;
        (*raiz)->balanco = 0;
        (*raiz) = u;
    }
    else
    {
        v = u->filhoEsq;
        u->filhoEsq = v->filhoDir;
        v->filhoDir = u;
        (*raiz)->filhoDir = v->filhoEsq;
        v->filhoEsq = (*raiz);
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

void insereNumaAVL(Palavra x, noh **raiz, int *alterou)
{
    if (*raiz == NULL)
    {
        *raiz = (noh *)malloc(sizeof(noh));
        (*raiz)->filhoEsq = NULL;
        (*raiz)->filhoDir = NULL;
        (*raiz)->chave = x;
        (*raiz)->balanco = 0;
        *alterou = 1;
    }
    else
    {
        if (strcmp(x.palavra, (*raiz)->chave.palavra) < 0)
        {
            insereNumaAVL(x, &((*raiz)->filhoEsq), alterou);
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
                    else
                        fprintf(stderr, "erro inesperado\n");
                }
            }
        }
        else if (strcmp(x.palavra, (*raiz)->chave.palavra) > 0)
        {
            insereNumaAVL(x, &((*raiz)->filhoDir), alterou);
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
                    else
                        fprintf(stderr, "erro inesperado\n");
                }
            }
        }
    }
}

int consulta(noh **raiz, char valor[31], FILE **output)
{
    if (raiz == NULL)
        return 0;
    noh *atual;
    atual = malloc(sizeof(noh));
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
            atual = atual->filhoDir;
        }
        else if (strcmp(valor, atual->chave.palavra) < 0)
        {
            fprintf(*output, "%s->", atual->chave.palavra);
            atual = atual->filhoEsq;
        }
    }
    fprintf(*output, "?]\n-\n");
    return 0;
}

int main(int argc, char *argv[])
{
    int h;
    noh *raiz = NULL;

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int qntd = 0, qntdsin = 0;
    char palavra[31], sinonimo[31];
    fscanf(input, "%i", &qntd);
    Palavra *palavras;
    palavras = (Palavra *)malloc(qntd * sizeof(Palavra));
    for (int i = 0; i < qntd; i++)
    {
        fscanf(input, "%s %i", palavra, &qntdsin);
        strcpy(palavras[i].palavra, palavra);
        palavras[i].len = qntdsin;
        palavras[i].sinonimos = (Sinonimo *)malloc(qntdsin * sizeof(Sinonimo));
        for (int x = 0; x < qntdsin; x++)
        {
            fscanf(input, "%s", sinonimo);
            strcpy(palavras[i].sinonimos[x].sinonimo, sinonimo);
        }
        insereNumaAVL(palavras[i], &raiz, &h);
    }

    fscanf(input, "%i", &qntd);
    for (int i = 0; i < qntd; i++)
    {
        fscanf(input, "%s", palavra);
        consulta(&raiz, palavra, &output);
    }

    fclose(input);
    fclose(output);

    return 0;
}
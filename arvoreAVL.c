#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NO *ArvAVL;

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

struct NO
{
    Palavra info;
    int alt;
    struct NO *esq;
    struct NO *dir;
};

ArvAVL *cria_ArvAVL()
{
    ArvAVL *raiz = (ArvAVL *)malloc(sizeof(ArvAVL));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

int maior(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

int altura_NO(struct NO *no)
{
    if ((no == NULL) || (no->esq == NULL && no->dir == NULL))
        return 0;
    else
        return 1 + maior(altura_NO(no->esq), altura_NO(no->dir));
}

int fatorBalanceamento_NO(struct NO *no)
{
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

void RotacaoLL(ArvAVL *raiz)
{
    struct NO *no;
    no = (*raiz)->esq;
    (*raiz)->esq = no->dir;
    no->dir = *raiz;
    (*raiz)->alt = maior(altura_NO((*raiz)->esq), altura_NO((*raiz)->dir)) + 1;
    no->alt = maior(altura_NO(no->esq), (*raiz)->alt) + 1;
    *raiz = no;
}

void RotacaoRR(ArvAVL *raiz)
{
    struct NO *no;
    no = (*raiz)->dir;
    (*raiz)->dir = no->esq;
    no->esq = (*raiz);
    (*raiz)->alt = maior(altura_NO((*raiz)->esq), altura_NO((*raiz)->dir)) + 1;
    no->alt = maior(altura_NO(no->dir), (*raiz)->alt) + 1;
    (*raiz) = no;
}

void RotacaoLR(ArvAVL *raiz)
{
    RotacaoRR(&(*raiz)->esq);
    RotacaoLL(raiz);
}

void RotacaoRL(ArvAVL *raiz)
{
    RotacaoLL(&(*raiz)->dir);
    RotacaoRR(raiz);
}

int insere_ArvAVL(ArvAVL *raiz, Palavra valor)
{
    int res;
    if (*raiz == NULL)
    {
        struct NO *novo;
        novo = (struct NO *)malloc(sizeof(struct NO));
        if (novo == NULL)
            return 0;
        novo->info = valor;
        novo->alt = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }
    struct NO *atual = *raiz;
    if (strcmp(valor.palavra, atual->info.palavra) < 0)
    {
        if ((res = insere_ArvAVL(&(atual->esq), valor)) == 1)
        {
            if (fatorBalanceamento_NO(atual) >= 2)
            {
                if (strcmp(valor.palavra, (*raiz)->esq->info.palavra) < 0)
                {
                    printf("RotacaoLL\n");
                    RotacaoLL(raiz);
                }
                else
                {
                    printf("RotacaoLR\n");
                    RotacaoLR(raiz);
                }
            }
        }
    }
    else
    {
        if (strcmp(valor.palavra, atual->info.palavra) > 0)
        {
            if ((res = insere_ArvAVL(&(atual->dir), valor)) == 1)
            {
                if (fatorBalanceamento_NO(atual) >= 2)
                {
                    if (strcmp((*raiz)->dir->info.palavra, valor.palavra) < 0)
                    {
                        printf("RotacaoRR\n");
                        RotacaoRR(raiz);
                    }
                    else
                    {
                        printf("RotacaoRL\n");
                        RotacaoRL(raiz);
                    }
                }
            }
        }
        else
        {
            printf("Valor duplicado!!\n");
            return 0;
        }
    }
    atual->alt = maior(altura_NO(atual->esq), altura_NO(atual->dir)) + 1;
    return res;
}

int consulta(ArvAVL *raiz, char valor[31], FILE **output)
{
    if (raiz == NULL)
        return 0;
    struct NO *atual = *raiz;
    printf("[");
    while (atual != NULL)
    {
        if (strcmp(valor, atual->info.palavra) == 0)
        {
            printf("%s]\n", valor);
            printf("%s", atual->info.sinonimos[0]);
            for (int i = 1; i < atual->info.len; i++)
            {
                printf(", %s", atual->info.sinonimos[i]);
            }
            printf("\n");
            return 1;
        }
        if (valor > atual->info.palavra)
        {
            printf("%s->", atual->info.palavra);
            atual = atual->dir;
        }
        else
        {
            printf("%s->", atual->info.palavra);
            atual = atual->esq;
        }
    }
    printf("?]\n-\n");
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    ArvAVL *raiz = cria_ArvAVL();
    int qntd = 0, qntdsin = 0;
    char palavra[31], sinonimo[31];
    fscanf(input, "%i", &qntd);
    Palavra *palavras;
    palavras = (Palavra *)malloc(qntd * sizeof(Palavra));
    for (int i = 0; i < qntd; i++)
    {
        fscanf(input, "%s %i", palavra, &qntdsin);
        strcpy(palavras[i].palavra, palavra);
        //printf("%s\n", palavra);
        palavras[i].len = qntdsin;
        palavras[i].sinonimos = (Sinonimo *)malloc(qntdsin * sizeof(Sinonimo));
        for (int x = 0; x < qntdsin; x++)
        {
            fscanf(input, "%s ", sinonimo);
            strcpy(palavras[i].sinonimos[x].sinonimo, sinonimo);
        }
        int v = insere_ArvAVL(raiz, palavras[i]);
    }

    fscanf(input, "%i", &qntd);
    for (int i = 0; i < qntd; i++)
    {
        fscanf(input, "%s", palavra);
        printf("%s\n", palavra);
        //consulta(raiz, palavra, &output);
    }

    fclose(input);
    fclose(output);

    return 0;
}
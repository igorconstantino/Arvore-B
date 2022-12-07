#include <stdio.h>
#include <stdlib.h>

#define MAX 4
#define MIN 2

typedef struct no
{
  int item[MAX + 1];
  int count;
  struct no *link[MAX + 1];
} No;

No *raiz;

No *criarNo(int item, No *filho)
{
  No *novoNo = (No *)malloc(sizeof(No));

  novoNo->item[1] = item;
  novoNo->count = 1;
  novoNo->link[0] = raiz;
  novoNo->link[1] = filho;

  return novoNo;
}

void InserirValor(int item, int pos, No *no, No *filho)
{
  int j = no->count;
  while (j > pos)
  {
    no->item[j + 1] = no->item[j];
    no->link[j + 1] = no->link[j];
    j--;
  }

  no->item[j + 1] = item;
  no->link[j + 1] = filho;
  no->count++;
}

void divideNo(int item, int *pval, int pos, No *no, No *filho, No **novoNo)
{
  int mediana, j;

  *novoNo = (No *)malloc(sizeof(No));
  mediana = pos > MIN ? MIN + 1 : MIN;

  j = mediana + 1;
  while (j <= MAX)
  {
    (*novoNo)->item[j - mediana] = no->item[j];
    (*novoNo)->link[j - mediana] = no->link[j];
    j++;
  }

  no->count = mediana;
  (*novoNo)->count = MAX - mediana;

  if (pos <= MIN)
    InserirValor(item, pos, no, filho);
  else
    InserirValor(item, pos - mediana, *novoNo, filho);

  *pval = no->item[no->count];
  (*novoNo)->link[0] = no->link[no->count];
  no->count--;
}

void limparBuffer()
{
  int c;
  while (((c = getchar()) == '\n' && c == EOF))
    ;
}
void escreveArvB(No *noSelecionado)
{
  int i;
  if (noSelecionado)
  {
    for (i = 0; i < noSelecionado->count; i++)
    {
      escreveArvB(noSelecionado->link[i]);
      printf("%d ", noSelecionado->item[i + 1]);
    }

    escreveArvB(noSelecionado->link[i]);
  }
}

int procura(No *noSelecionado, int val)
{
  int i;
  if (noSelecionado)
  {
    for (i = 0; i < noSelecionado->count; i++)
    {
      if (procura(noSelecionado->link[i], val))
        return 1;

      if (noSelecionado->item[i + 1] == val)
        return 1;
    }

    if (procura(noSelecionado->link[i], val))
      return 1;
  }

  return 0;
}

int setNo(int item, int *pval, No *no, No **filho)
{
  int pos, ret;
  if (!no)
  {
    *pval = item;
    *filho = NULL;
    return -1;
  }

  if (item < no->item[1])
    pos = 0;
  else
  {
    for (pos = no->count; (item < no->item[pos] && pos > 1); pos--)
      ;

    if (item == no->item[pos]) // Valor duplicado
      return -2;
  }

  ret = setNo(item, pval, no->link[pos], filho);
  if (ret == -2)
    return -2;
  else if (ret)
  {
    if (no->count < MAX)
      InserirValor(*pval, pos, no, *filho);
    else
    {
      divideNo(*pval, pval, pos, no, *filho, filho);
      return 1;
    }
  }

  return 0;
}

void inserir(int item)
{
  int flag, i;
  No *filho;

  flag = setNo(item, &i, raiz, &filho);
  if (flag && flag != -2)
    raiz = criarNo(i, filho);

  if (flag != -2)
    printf("\n--> Valor inserido com sucesso!\n\n");
  else
    printf("\n!--> nao permite valor duplicado!\n\n");
}

/*===========================================Funçõs para printar a arvore===========================================*/
int getTamanhoArvB(No *root) // Calculando nivel da árvore B
{
  int height;

  if (!root)
    return -1;

  height = getTamanhoArvB(root->link[0]) + 1;

  return height;
}

void escreveNoCompleto(No *no) // Escrever o nó
{
  int i;
  for (i = 0; i < no->count; i++)
    printf("%d ", no->item[i + 1]);
  printf("      ");
}

void escreveNivelArvB(No *root, int nivel) // Escrevendo todos os nós da árvore
{
  if (!root)
    return;

  if (nivel == 0)
    escreveNoCompleto(root);
  else if (nivel > 0)
    for (int i = 0; i <= root->count; i++)
      escreveNivelArvB(root->link[i], nivel - 1);
}

void escreveArvBPorNivel(No *root) // Função completa para printar a árvore por nível
{
  int height = getTamanhoArvB(root);
  int i;
  for (i = 0; i <= height; i++)
  {
    if (i == 0)
      printf("RAIZ:\t\t");
    else
      printf("\n\nNIVEL %02d:\t", i);

    escreveNivelArvB(root, i);
  }
}
/*===========================================Funçõs para printar a arvore===========================================*/


int main()
{
  int input, op, item;

  do
  {
    do
    {
      printf("\n======== MENU ========");
      printf("\n(0) Sair");
      printf("\n(1) Inserir na Arvore-B");
      printf("\n(2) Buscar na Arvore-B");
      printf("\n(3) Mostrar Arvore-B");

      printf("\n\n-> Insira a opcao desejada: ");
      input = scanf(" %d", &op);

      if (!input)
        limparBuffer();

      if (!input || op < 0 || op > 3)
        printf("\n!-> Insira uma opcao válida!\n\n");
    } while (!input|| op < 0 || op > 3);

    switch (op)
    {
    case 1:
      printf("\n--> Insira o valor a ser inserido: ");
      input = scanf(" %d", &item);

      if (!input)
      {
        limparBuffer();
        printf("\nInsira um valor valido!\n\n");
      }
      else
        inserir(item);

      break;

    case 2:
      printf("\n--> Insira o valor a ser buscado: ");
      input = scanf(" %d", &item);

      if (!input)
      {
        limparBuffer();
        printf("\nInsira um valor valido!\n\n");
      }
      else if (procura(raiz, item))
        printf("\n--> Valor encontrado!\n\n");
      else
        printf("\n--> O valor nao foi encontado!\n\n");

      break;

    case 3:
      if (!raiz)
        printf("\n--> Arvore vazia!\n\n");
      else
      {
        printf("\n--> Arvore-B (Altura %d)\n\n", getTamanhoArvB(raiz));
        escreveArvBPorNivel(raiz);
        printf("\n\n");
      }

      break;
    }
  } while (op != 0);

  return 0;
}
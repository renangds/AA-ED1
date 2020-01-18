#define NOME 81
#define TEL 11

typedef struct contato{
  char nome[NOME];
  char tel[TEL];
}Contato;

typedef struct lista{
  Contato* contato;
  struct lista* prox;
}Lista;

Lista* iniciar();
Lista* inserir_contatos(Lista* lc, char* nome, char* telefone);
Lista* busca(Lista* lc, char* nome);
Lista* remover(Lista* lc, Lista* r);
Lista* opcao_remover(Lista* lc);
Lista* opcao_criar_contato(Lista* lc);
Lista* opcoes(Lista* lc);
Lista* leitura();
void escrita(Lista* lc);
void programa();
void imprimir(Lista* lc);

Lista* iniciar(){
  Lista* nova = (Lista*)malloc(sizeof(Lista));
  return nova;
}

Lista* inserir_contatos(Lista* lc, char* nome, char* telefone){
  Contato* c = (Contato*)malloc(sizeof(Contato));
  strcpy(c->nome, nome);
  strcpy(c->tel, telefone);

  if(!lc->contato){
    lc->contato = c;
    return lc;
  }

  Lista* p = lc;
  Lista* aux;
  Lista* nova = iniciar();
  nova->prox = NULL;
  nova->contato = c;

  while(p && strcmp(c->nome, p->contato->nome) >= 0){
    aux = p;
    p = p->prox;
  }

  if(p == lc){
    nova->prox = lc;
    return nova;
  }

  if(!p){
    aux->prox = nova;
    return lc;
  }

  nova->prox = p;
  aux->prox = nova;

  return lc;
}

Lista* busca(Lista* lc, char* nome){
  Lista* p = lc;
  while(p && p->contato){
    if(strcmp(nome, p->contato->nome) == 0) return p;
    p = p->prox;
  }
  return NULL;
}

Lista* remover(Lista* lc, Lista* r){
  Lista* aux = lc;

  if(r == lc){
    aux = r->prox;
    lc = aux;
    free(r);
    return lc;
  }

  while(aux->prox!=r){
    aux = aux->prox;
  }

  if(!r->prox){
    aux->prox = NULL;
    free(r);
    return lc;
  }

  aux->prox = r->prox;
  free(r);

  return lc;
}

void imprimir(Lista* lc){
  Lista* p = lc;
  printf("----------------------\n");
  while(p && p->contato){
    printf("%s %s\n", p->contato->nome, p->contato->tel);
    p = p->prox;
  }
  printf("----------------------\n");
}

//Funções menores para as opções do programa

Lista* opcao_remover(Lista* lc){
  char nome[NOME];
  int opcao;

  printf("Digite o nome da busca:\n");
  scanf("%s", nome);
  Lista* r = busca(lc, nome);
  if(r){
    printf("Deseja remover? 1.Sim | 2.Não\n");
    scanf("%d", &opcao);
    if(opcao == 1){
      lc = remover(lc, r);
    } else{
      printf("Abortado\n");
    }
  } else{
    printf("Não encontrado\n");
  }

  return lc;
}

Lista* opcao_criar_contato(Lista* lc){
  char nome[NOME];
  char telefone[TEL];

  printf("Digite o nome e telefone\n");
  scanf("%s", nome);
  scanf("%s", telefone);
  lc = inserir_contatos(lc, nome, telefone);

  return lc;
}

//Menu do programa

Lista* opcoes(Lista* lc){
  int opcao;

  while(opcao!=9){
    printf("Selecione a opção: 1.Listar | 2.Buscar | 3.Criar | 9. Sair\n");
    scanf("%d", &opcao);

    switch(opcao){
      case 1:
        imprimir(lc);
        break;
      case 2:
        lc = opcao_remover(lc);
        break;
      case 3:
        lc = opcao_criar_contato(lc);
        break;
      case 9:
        printf("Encerrando\n");
        break;
      default:
        printf("Opção inválida\n");
    }
  }

  return lc;
}

//Funções para leitura e escrita da lista de contatos

Lista* leitura(){
  FILE* arq;
  Lista* lnova = iniciar();
  char* nome_arquivo = "contatos.data";
  char nome[NOME], tel[TEL];

  arq = fopen(nome_arquivo, "r");

  if(!arq){
    printf("Lista de contatos inexistente!\n");

    return lnova;
  }

  while((fscanf(arq, "%s %s\n", nome, tel)) != EOF){
    lnova = inserir_contatos(lnova, nome, tel);
  }

  fclose(arq);

  return lnova;
}

void escrita(Lista* lc){
  FILE* arq;
  char* nome_arquivo = "contatos.data";

  if((arq = fopen(nome_arquivo, "w+")) == NULL){
    printf("Não foi possível gerar o arquivo");
  } else{
    Lista* p = lc;

    while(p){
      fprintf(arq, "%s %s\n", p->contato->nome, p->contato->tel);
      p = p->prox;
    }

    fclose(arq);
  }
}

void programa(){
  Lista* lista_contatos = leitura();

  lista_contatos = opcoes(lista_contatos);

  if(lista_contatos->contato) escrita(lista_contatos);

  free(lista_contatos);
}

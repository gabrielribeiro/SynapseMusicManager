/*
Nome: Gabriel Ribeiro         RA: 11100575
Nome: Guilherme Titschkoski   RA: 11157237
Nome: Lucas Argate            RA: 11164126
Opcionais funcionando: Opcional 1
                       Opcional 3
                       Opcional 5
                       Opcional 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"

typedef struct Musica
{
   char Titulo[20];
   char Interprete[20];
   char Album[20];
   char Modificado[20];
   struct Musica *Prox;
   int Index;
}Musica;

int  TelaPrincipal(Musica *lista);
void PromptInserir(Musica *lista);
void Inserir(Musica *lista, char titulo[20], char interprete[20], char album[20]);
void LimparLinha(int linha);
void MostrarGrid(Musica *lista, int index);
void Buscar(Musica *lista, char termo[]);
int  AlterarOuDeletar();
void Alterar (Musica *lista, int index);
void Deletar (Musica *lista, int index);
void GravarEmArquivo(Musica *lista);

int totalMusicas = 0;

int main(int argc, char *argv[])
{   
    gotoxy(3, 1);  printf("Menu: Use as setas Esq, Dir e Enter para Selecionar uma opcao.");
    gotoxy(3, 2);  printf("Musicas: Use as setas Cima, Baixo e Enter para Alterar ou Deletar uma Musica.");
    Musica *lista, *aux;
    Musica data;

    lista = (struct Musica *) malloc (sizeof (Musica));
    strcpy(lista->Titulo, "");
    lista->Prox = NULL;

    FILE *f;

    if ((f = fopen("musics.dat","r+b")) != NULL) 
    {
        while (!feof(f)) {
            fread( &data, sizeof(struct Musica), 1, f);
            if (!feof(f)) {
                Inserir((struct Musica *)lista, data.Titulo, data.Interprete, data.Album);
            }
        }
    }

    int opcao = 3;
    char termo[20];
    
    MostrarGrid((struct Musica *)lista, 0);
    
    do
    {
        opcao = TelaPrincipal((struct Musica *)lista);
    
        fflush(stdin);
        
        if(opcao > 0)
        {
            switch(opcao)
            {
                case 1:
                    PromptInserir((struct Musica *)lista);
                    GravarEmArquivo((struct Musica *)lista);
                    MostrarGrid((struct Musica *)lista, 0);
                    break;
                case 2:
                    gotoxy(23, 3);
                    gets(termo);
                    Buscar((struct Musica *)lista, termo);
                    break;
            }
        }
        else
        {
            int opcaoB = AlterarOuDeletar();
            switch(opcaoB)
            {
                case 1: //ALTERAR
                     Alterar ((struct Musica *)lista, opcao * -1);
                     break;
                case 2: //DELETAR
                     Deletar ((struct Musica *)lista, opcao * -1);
                     break;
            }
            
            int i;
            for(i = 4; i < 30; i++){
                  LimparLinha(i);
            }
            
            GravarEmArquivo((struct Musica *)lista);
            MostrarGrid((struct Musica *)lista, 0);
        }
    }
    while (opcao < 3);
        
    return 0;
}

int TelaPrincipal(Musica *lista)
{         
     gotoxy(3, 3);  printf(">");  
     gotoxy(5, 3);  printf("Nova");
     gotoxy(15, 3);  printf("Buscar: ");
     gotoxy(60, 3);  printf("Sair");
         
     int opcao = 1;
     int index = 0;
     
     while(1)
     {
         if (kbhit())
         {
             int cmd = getch();
             
             switch(cmd)
             {
                  case 75: //esq
                       if (index == 0 && opcao > 1)
                          opcao--;
                       break;
                  case 77: //dir
                       if (index == 0 && opcao < 3)
                          opcao++;
                       break;
                  case 72: //Cima
                       if (index >= 1 && index <= totalMusicas)
                          index--;
                       break;
                  case 80: //Baixo
                       if (index >= 0 && index < totalMusicas)
                          index++;
                       break;
                  case 13: //Enter
                       if(index == 0)
                          return opcao; //Sair da função
                       else
                          return index * -1;
                       break;
             }
             
             gotoxy(3, 3);
             printf(" ");
             gotoxy(13, 3);
             printf(" ");
             gotoxy(58, 3);
             printf(" ");
            
             if(index == 0)
             {
                 switch(opcao)
                 {
                      case 1:
                           gotoxy(3, 3);
                           break;
                      case 2:
                           gotoxy(13, 3);
                           break;
                      case 3:
                           gotoxy(58, 3);
                           break;
                 }
                 
                 printf(">");
             }

             MostrarGrid((struct Musica *)lista, index);
         }
     }
}

void PromptInserir (Musica *lista)
{
   char titulo[20];
   char interprete[20];
   char album[20];

   fflush(stdin);

   LimparLinha(5);
   LimparLinha(6);

   gotoxy(3, 5); printf("Inserindo Nova Musica");
   
   gotoxy(3, 6); printf("Titulo: ");
   gets(titulo);
   LimparLinha(6); gotoxy(3, 5);
   printf("Inserindo Nova Musica: %s", titulo);
   
   gotoxy(3, 6); printf("Interprete: ");
   gets(interprete);
   LimparLinha(6); gotoxy(3, 5);
   printf("Inserindo Nova Musica: %s - %s", titulo, interprete);
   
   gotoxy(3, 6); printf("Album: ");
   gets(album);
   LimparLinha(6); gotoxy(3, 5);
   printf("Inserindo Nova Musica: %s - %s, %s", titulo, interprete, album);
   
   Inserir((struct Musica *)lista, titulo, interprete, album);
   
   LimparLinha(5);
   LimparLinha(6);
   gotoxy(3, 5);
   printf("Sucesso! Musica Inserida: %s - %s, %s", titulo, interprete, album);
}

void Inserir(Musica *lista, char titulo[20], char interprete[20], char album[20])
{
   Musica *nova;
   nova = (struct Musica *) malloc (sizeof (Musica));
      
   strcpy(nova->Titulo, titulo);
   strcpy(nova->Interprete, interprete);
   strcpy(nova->Album, album);
   
   Musica *aux;
   for (aux = lista; aux != NULL; aux = aux->Prox) 
   {
       if(aux->Prox == NULL) //Nova Música é maior que todos
       {
           nova->Prox = NULL;
           aux->Prox = nova;
           break;
       }
       else
       {
          if(strcmpi(nova->Titulo, aux->Prox->Titulo) > 0) //Nova Musica é Maior
          {
              continue;
          }
          else //Nova Musica é Menor Ou Igual
          {
              nova->Prox = aux->Prox;
              aux->Prox = nova;
              break;
          }
       }
   }
   
   int index = 0;
   for (aux = lista; aux != NULL; aux = aux->Prox) 
   {
       aux->Index = index;
       index += 1;
   }
   totalMusicas = index;
}

void LimparLinha(int linha)
{
    fflush(stdin);
    int i;
    for(i = 1; i < 80; i++)
    {
        gotoxy(i, linha); printf(" ");
    }
}

void MostrarGrid(Musica *lista, int index)
{
   int coluna;
   
   if(lista->Prox != NULL)
   {       
       for(coluna = 3; coluna < 79; coluna++){
             gotoxy(coluna, 7); printf("%c", 196);
       }
       
       gotoxy(2, 7); printf("%c", 218);
       gotoxy(20, 7); printf("%c", 194);
       gotoxy(40, 7); printf("%c", 194);
       gotoxy(60, 7); printf("%c", 194);
       gotoxy(79, 7); printf("%c", 191);
             
       gotoxy(9, 8); printf("Titulo");
       gotoxy(26, 8); printf("Interprete");
       gotoxy(48, 8); printf("Album");
       gotoxy(63, 8); printf("Modificado em:");

       gotoxy(02, 8); printf("%c", 179);
       gotoxy(20, 8); printf("%c", 179);
       gotoxy(40, 8); printf("%c", 179);
       gotoxy(60, 8); printf("%c", 179);
       gotoxy(79, 8); printf("%c", 179);
       
       for(coluna = 3; coluna < 79; coluna++){
             gotoxy(coluna, 9); printf("%c", 196);
       }
       
       gotoxy(02, 9); printf("%c", 195);
       gotoxy(20, 9); printf("%c", 197);
       gotoxy(40, 9); printf("%c", 197);
       gotoxy(60, 9); printf("%c", 197);
       gotoxy(79, 9); printf("%c", 180);
       
       int i = 1;
       Musica *aux;
       for (aux = lista->Prox; aux != NULL; aux = aux->Prox) 
       {
           int linha = 9 + i;
           LimparLinha(linha);

           if(index > 0 && aux->Index == index)
           {
               textcolor(CYAN);
           }
          
           gotoxy(3, linha); printf ("%s", aux->Titulo);
           gotoxy(21, linha); printf ("%s", aux->Interprete);
           gotoxy(41, linha); printf ("%s", aux->Album);
           gotoxy(61, linha); printf ("%i", aux->Index);
           
           if(index > 0 && aux->Index == index)
           {
               textcolor(WHITE);
           }
           
           gotoxy(02, linha); printf("%c", 179);
           gotoxy(20, linha); printf("%c", 179);
           gotoxy(40, linha); printf("%c", 179);
           gotoxy(60, linha); printf("%c", 179);
           gotoxy(79, linha); printf("%c", 179);
           
           i++;
       }
       
       int linha = 9 + i;
       
       for(coluna = 3; coluna < 79; coluna++){
             gotoxy(coluna, linha); printf("%c", 196);
       }
       
       gotoxy(02, linha); printf("%c", 192);
       gotoxy(20, linha); printf("%c", 193);
       gotoxy(40, linha); printf("%c", 193);
       gotoxy(60, linha); printf("%c", 193);
       gotoxy(79, linha); printf("%c", 217);
   }
   else
   {
       gotoxy(28, 5); printf ("Nenhuma Musica Encontrada.");
   }
}

void Buscar(Musica *lista, char termo[])
{    
    int i;
    for(i = 3; i < 30; i++){
          LimparLinha(i);
    }

    if(lista != NULL)
    {
       Musica *buscaTitulo, *buscaInterprete, *swap;
       buscaTitulo = (struct Musica *) malloc (sizeof (Musica));
       buscaInterprete = (struct Musica *) malloc (sizeof (Musica));
       swap = (struct Musica *) malloc (sizeof (Musica));
       
       LimparLinha(5);
       gotoxy(3, 5);printf("Busca por Titulo: %s", termo);
             
       int index = 1;
       int linha = 7;
       Musica *aux;
       
       //Busca por Título
       for (aux = lista->Prox; aux != NULL; aux = aux->Prox) 
       {
           if(strcmpi(aux->Titulo, termo) == 0)
           {
               linha += index;
               LimparLinha(linha);
               gotoxy(3, linha); printf ("%d - %s", index, aux->Titulo);
               index++;
           }
       }
       
       linha += 3;
       gotoxy(3, linha); printf("Busca por Interprete: %s", termo);
       linha += 2;
       
       //Busca por Interprete
       for (aux = lista->Prox; aux != NULL; aux = aux->Prox) 
       {
           if(strcmpi(aux->Interprete, termo) == 0)
           {
               linha += index;
               LimparLinha(linha);
               gotoxy(3, linha); printf ("%d - %s", index, aux->Titulo);
               index++;
           }
       }
    }
    else
    {
          gotoxy(3, 7); printf ("Nenhuma musica encontrada.");
    }
}

int AlterarOuDeletar()
{    
     LimparLinha(5);
     LimparLinha(6);
    
    gotoxy(03, 5);  printf(">");  
    gotoxy(05, 5);  printf("Alterar");
    gotoxy(20, 5);  printf("Deletar");
     
    int opcao = 1;
    while(1)
    {
        if (kbhit())
        {
            int cmd = getch();
            
            switch(cmd)
            {
                case 75: //esq
                    if (opcao > 1)
                      opcao--;
                    break;
                case 77: //dir
                    if (opcao < 2)
                      opcao++;
                    break;
                case 13: //Enter
                      return opcao; //Sair da função
                   break;
             }
             
             gotoxy(3, 5);
             printf(" ");
             gotoxy(18, 5);
             printf(" ");

             switch(opcao)
             {
                  case 1:
                       gotoxy(3, 5);
                       break;
                  case 2:
                       gotoxy(18, 5);
                       break;
             }
             
             printf(">");
        }
    }
    
    int i;
    for(i = 6; i < 30; i++){
          LimparLinha(i);
    }
}

void Alterar (Musica *lista, int index)
{
   char titulo[20];
   char interprete[20];
   char album[20];

   fflush(stdin);

   LimparLinha(5);
   LimparLinha(6);

   gotoxy(3, 5); printf("Alterando Musica");
   
   gotoxy(3, 6); printf("Titulo: ");
   gets(titulo);
   LimparLinha(6); gotoxy(3, 5);
   printf("Alterando Musica: %s", titulo);
   
   gotoxy(3, 6); printf("Interprete: ");
   gets(interprete);
   LimparLinha(6); gotoxy(3, 5);
   printf("Alterando Musica: %s - %s", titulo, interprete);
   
   gotoxy(3, 6); printf("Album: ");
   gets(album);
   LimparLinha(6); gotoxy(3, 5);
   printf("Alterando Musica: %s - %s, %s", titulo, interprete, album);
   
   Musica *aux, *a, *b, *c;
   for (aux = lista; aux->Prox != NULL; aux = aux->Prox) 
   {
       a = aux;
       b = aux->Prox;
       c = aux->Prox->Prox;
       
       if(b->Index == index)
       {
           free(b);
           a->Prox = c;
           break;
       }
   }

   int newIndex = 0;
   for (aux = lista; aux != NULL; aux = aux->Prox) 
   {
       aux->Index = newIndex;
       newIndex += 1;
   }
   
   Inserir(lista, titulo, interprete, album);
   
   LimparLinha(5);
   LimparLinha(6);
   gotoxy(3, 5);
   printf("Sucesso! Musica Alterada: %s - %s, %s", titulo, interprete, album);
}

void Deletar (Musica *lista, int index)
{
   //DELETANDO
   Musica *aux, *a, *b, *c;
   for (aux = lista; aux->Prox != NULL; aux = aux->Prox) 
   {
       a = aux;
       b = aux->Prox;
       c = aux->Prox->Prox;
       
       if(b->Index == index)
       {
           free(b);
           a->Prox = c;
           break;
       }
   }

   int newIndex = 0;
   for (aux = lista; aux != NULL; aux = aux->Prox) 
   {
       aux->Index = newIndex;
       newIndex += 1;
   }
   
   gotoxy(20,22); printf("ordenou");
   
   totalMusicas = newIndex;
   
   LimparLinha(5);
   LimparLinha(6);
   gotoxy(3, 5);
   printf("Sucesso! Musica Deletada!");
}

void GravarEmArquivo(Musica *lista)
{
    Musica *aux;
    FILE *f = fopen("musics.dat", "w+b");
    if (f != NULL)
    {
        for (aux = lista->Prox; aux != NULL; aux = aux->Prox) 
        {
             fwrite((Musica *)aux, sizeof(Musica), 1, f);
        }
        fclose(f);
    }
}

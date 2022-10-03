#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0


typedef struct Letra{
    char caracter;
    struct Letra *proximo;
}Letra;

typedef struct Texto{
    Letra *inicio;
    Letra *fim;
    struct Texto *proximo;
}Texto;

typedef struct{
    Texto *inicio;
    Texto *fim;
}Lista_Texto;

typedef struct Cabecalho{
    Texto *atributo;
    Texto *valor;
    struct Cabecalho *proximo;
}Cabecalho;

typedef struct{
    Cabecalho *inicio;
    Cabecalho *fim;
}Lista_Cabecalho;

typedef struct{
    int tipo;//1-Requisição/ 2-Resposta
    Texto *linha1_posicao1;
    Texto *linha1_posicao2;
    Texto *linha1_posicao3;
    Lista_Cabecalho *cabecalho;
    Texto *corpo;
}Http;


int len_texto(Texto *p_texto);


void show_texto(Texto *p_texto){
    if(p_texto != NULL){
        Letra *p_letra;
        p_letra = p_texto->inicio;
        while(p_letra != NULL){
            printf("%c", p_letra->caracter);
            p_letra = p_letra->proximo;
        }
    }else{
        printf("Vazio\n");
    }
}

void show_lista_de_textos(Lista_Texto *lista){
    int i =0;
    if(lista != NULL){
        Texto *texto;
        texto = lista->inicio;
        while(texto != NULL){
            printf("LINHA %d: ",i);
            show_texto(texto);
            printf("\n");
            texto = texto->proximo;
            i++;
        }
    }else{
        printf("Lista Vazia");
    }
}

void show_header(Http *http){
    printf("\n----------------    INTERPRETADOR PROTOCOLO HTTP   ----------------");
    if (http->tipo == 1){
        printf("\nTipo da Mensagem: Requisição");
        printf("\nMetodo: ");
        show_texto(http->linha1_posicao1);
        printf("\nURL: ");
        show_texto(http->linha1_posicao2);
        printf("\nVersão: ");
        show_texto(http->linha1_posicao3);
    }else{
        printf("\nTipo da Mensagem: Resposta");
        printf("\nVersão: ");
        show_texto(http->linha1_posicao1);
        printf("\nStatus: ");
        show_texto(http->linha1_posicao2);
        printf(" - ");
        show_texto(http->linha1_posicao3);
    }
    printf("\n");
    
}

void show_cabecalho(Http *http){
    printf("\n-------------------------    CABECALHO   --------------------------\n");
    
    if(http->cabecalho != NULL){
        Cabecalho *item;
        item = http->cabecalho->inicio;
        while(item != NULL){
            show_texto(item->atributo);
            printf(" :: ");
            show_texto(item->valor);
            printf("\n");
            item = item->proximo;
        }
        printf("\n");
    }else{
        printf("Lista Vazia");
    }
    if (len_texto(http->corpo) > 0){
        printf("\n---------------------------    CORPO   ----------------------------\n");
        show_texto(http->corpo);
    }
    printf("\n");
}


Lista_Cabecalho *incluir_cabecalho_na_lista(Cabecalho *p_cabecalho, Lista_Cabecalho *p_lista){
    p_cabecalho->valor->inicio = p_cabecalho->valor->inicio->proximo;
    if (p_lista == NULL){
        p_lista = malloc(sizeof(Lista_Cabecalho));
        p_lista->inicio = p_cabecalho;
        p_lista->fim = p_cabecalho;
    }else{
        p_lista->fim->proximo = p_cabecalho;
        p_lista->fim = p_cabecalho;
    }
    return p_lista;
}


Lista_Texto *incluir_texto_na_lista(Texto *p_texto, Lista_Texto *p_lista){
    if (p_lista == NULL){
        p_lista = malloc(sizeof(Lista_Texto));
        p_lista->inicio = p_texto;
        p_lista->fim = p_texto;
    }else{
        p_lista->fim->proximo = p_texto;
        p_lista->fim = p_texto;
    }
    return p_lista;
}

Texto *incluir_caractere_no_texto(char c, Texto *p_texto){
    Letra *p_letra;
    p_letra = malloc(sizeof(Letra));
    p_letra->caracter = c;
    p_letra->proximo = NULL;
    if (p_texto == NULL){
        p_texto = malloc(sizeof(Texto));
        p_texto->inicio = p_letra;
        p_texto->fim = p_letra;
    }else{
        p_texto->fim->proximo = p_letra;
        p_texto->fim = p_letra;
    }
    return p_texto;
}

Texto *carregar_arquivo(FILE *p_file){
    char caracter_atual;
    Texto *texto;
    texto = NULL;
    while((caracter_atual = fgetc(p_file)) != EOF){
        texto = incluir_caractere_no_texto(caracter_atual,texto);
    }
    return texto;
}

Lista_Texto *split_texto(Texto *texto_inicial, char delimitador){
    Lista_Texto *lista_com_as_linhas = NULL;
    Texto *texto_antes = NULL;
    if(texto_inicial != NULL){
        Letra *letra_atual;
        letra_atual = texto_inicial->inicio;
        while(letra_atual != NULL){
            if(letra_atual->proximo == NULL){
                texto_antes = incluir_caractere_no_texto(letra_atual->caracter, texto_antes);
                lista_com_as_linhas = incluir_texto_na_lista(texto_antes, lista_com_as_linhas);
            }else if (letra_atual->caracter != delimitador){
                texto_antes = incluir_caractere_no_texto(letra_atual->caracter, texto_antes);
            }else{
                lista_com_as_linhas = incluir_texto_na_lista(texto_antes, lista_com_as_linhas);
                texto_antes = NULL;
            }
            letra_atual = letra_atual->proximo;
        }
    }else{
        printf("Vazio!");
    }
    return lista_com_as_linhas;
}

Texto *pegar_texto_da_lista(Lista_Texto *lista, int posicao){
    int i=0;
    Texto *resultado, *texto;
    if(lista != NULL){
        texto = lista->inicio;
        while(texto != NULL){
            if (i == posicao){
                resultado = texto;
                return texto;
            }
            texto = texto->proximo;
            i++;
        }
    }else{
        printf("Lista Vazia");
        resultado = NULL;
    }
    return resultado;
}

Lista_Texto *split_texto_na_primeira_ocorrencia(Texto *texto_inicial,char delimitador){
    Lista_Texto *lista_com_as_linhas = NULL;
    Texto *texto_antes = NULL;
    if (texto_inicial != NULL){
        Texto *texto_depois = malloc(sizeof(Texto));
        texto_depois->fim = texto_inicial->fim;
        Letra *letra_atual = texto_inicial->inicio;
        while(letra_atual != NULL){
            if(letra_atual->caracter != delimitador){
                texto_antes = incluir_caractere_no_texto(letra_atual->caracter, texto_antes);
            }else{
                texto_depois->inicio = letra_atual->proximo;
                lista_com_as_linhas = incluir_texto_na_lista(texto_antes,lista_com_as_linhas);
                lista_com_as_linhas = incluir_texto_na_lista(texto_depois,lista_com_as_linhas);
                return lista_com_as_linhas;
            }
            letra_atual=letra_atual->proximo;
        }
        return incluir_texto_na_lista(texto_antes, lista_com_as_linhas);
    }else{
        printf("Vazio!");
        return NULL;
    }
}

int len_texto(Texto *p_texto){
    int count = 0;
    if(p_texto != NULL){
        Letra *p_letra;
        p_letra = p_texto->inicio;
        while(p_letra != NULL){
            count++;
            p_letra = p_letra->proximo;
        }
    }else{
        return 0;
    }
    return count;
}

char *get_string_do_texto(Texto *texto){
    char *string = NULL;
    if (texto != NULL){
        int tamanha_texto = len_texto(texto);
        int i;
        string = malloc(sizeof(char[tamanha_texto]));
        Letra *letra_agora = texto->inicio;
        for(i = 0; i< tamanha_texto; i++){
            string[i] = letra_agora->caracter;
            letra_agora=letra_agora->proximo;
        }
    }else{
        return NULL;
    }
    return string;
}

void verifica_tipo_da_mensagem(Http *http,char *string){
    int diff = strncmp("HTTP", string, 4);
    if(diff == 0){
        http->tipo = 2;
    }else{
        http->tipo = 1;
    }
}

Http *carregar_http_header(Http *http, Texto *texto_arquivo){
    Lista_Texto *lista_das_linhas = split_texto_na_primeira_ocorrencia(texto_arquivo, '\n');
    Texto *primeira_linha = pegar_texto_da_lista(lista_das_linhas, 0);
    Lista_Texto *lista_pocisao_primeira_linha = split_texto_na_primeira_ocorrencia(primeira_linha, ' ');
    http->linha1_posicao1 = pegar_texto_da_lista(lista_pocisao_primeira_linha, 0);
    lista_pocisao_primeira_linha = split_texto_na_primeira_ocorrencia(pegar_texto_da_lista(lista_pocisao_primeira_linha, 1),' ');
    http->linha1_posicao2 = pegar_texto_da_lista(lista_pocisao_primeira_linha, 0);
    http->linha1_posicao3 = pegar_texto_da_lista(lista_pocisao_primeira_linha, 1);
    verifica_tipo_da_mensagem(http, get_string_do_texto(http->linha1_posicao1));
    return http;
}

int incluir_atributo_cabecalho(Http *http, Texto *linha_do_atributo){

    if (len_texto(linha_do_atributo)> 2){
        Lista_Texto * lista_campos_cabecalho = split_texto_na_primeira_ocorrencia(linha_do_atributo, ':');
        Cabecalho *cabecalho_item = malloc(sizeof(Cabecalho));
        cabecalho_item->atributo = pegar_texto_da_lista(lista_campos_cabecalho, 0);
        cabecalho_item->valor = pegar_texto_da_lista(lista_campos_cabecalho, 1);
        http->cabecalho = incluir_cabecalho_na_lista(cabecalho_item, http->cabecalho);
        return TRUE;
    }else{
        return FALSE;
    }
}

Http *carregar_http_cabecalho(Http *http, Texto *texto_arquivo){
    Lista_Texto *lista_das_linhas = split_texto_na_primeira_ocorrencia(texto_arquivo, '\n');
    Texto *texto_cabecalho = pegar_texto_da_lista(lista_das_linhas, 1);
    int flag_run = TRUE;
    while(flag_run == TRUE){
        lista_das_linhas = split_texto_na_primeira_ocorrencia(texto_cabecalho, '\n');
        texto_cabecalho = pegar_texto_da_lista(lista_das_linhas, 0);
        flag_run = incluir_atributo_cabecalho(http, texto_cabecalho);
        texto_cabecalho = pegar_texto_da_lista(lista_das_linhas, 1);
    }
    http->corpo = texto_cabecalho;
    return http;
}

Http *carregar_http(Texto *texto_arquivo){
    Http *http = malloc(sizeof(Http));
    http = carregar_http_header(http, texto_arquivo);
    http = carregar_http_cabecalho(http, texto_arquivo);
    return http;
}

Texto *pegar_valor_atributo(Http *http, char *atributo){
    Texto *response = NULL;
    if(http->cabecalho != NULL){
        Cabecalho *item;
        item = http->cabecalho->inicio;
        while(item != NULL){
            if(strcmp(get_string_do_texto(item->atributo),atributo) == 0){
               response =  item->valor;
            }
            item = item->proximo;
        }
        printf("\n");
    }else{
        printf("Lista Vazia");
    }
    return response;
}

void show_protocolo(Http *http){
    show_header(http);
    show_cabecalho(http);
}

int main(int argc, char **argv){
    FILE *p_file;
    p_file = fopen(argv[1], "r");
    if (p_file != NULL){
        printf("Abrindo arquivo: %s\n",argv[1]);
        Texto *arquivo_completo = carregar_arquivo(p_file);
        Http *protocolo_http = carregar_http(arquivo_completo);
        show_protocolo(protocolo_http);
    }else{
        printf("Não foi possivel abrir o arquivo");
    }
    return 0;
}
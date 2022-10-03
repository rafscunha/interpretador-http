## **Proposta de Resolução do Interpretador do protocolo HTTP/1.1**

##Detalhamento do Exercício:
Deve-se elaborar um interpretador do protocolo HTTP, utilizando qualquer linguagem de programação.
O interpretador deve reconhecer o tipo de mensagem, métodos, status, versão do protocolo e url.
Deve tambem armazenar o cabeçalho da requisição de forma que se possa pesquisar um atributo especifico do mesmo, assim como o corpo da mensagem.

#Detalhamento sobre a Resolução em C

Essa proposta visa criar uma estrutura dinâmica para interpretar o protocolo, utilizando listas encadeadas.
* A leitura do arquivo foi realizada utilizando o metodo fgetc, onde foi feita a leitura de caracter por caracter e adicionando respectivamente em uma lista encadeada de caracteres.
* A struct Letra, possui dois campos, um é propriamente o caractere e outro, aponta para o proxima Letra.
* Existe tambem a struct Texto que guarda os ponteiro so inicio e fim de cada Texto - Com essa estrutura eu posso armazenar pedaços dessa lista incial obtida na leitura do arquivo, podendo implementar o split.
* Quando realiza o split de um Texto, gera um vetor de Textos, para isso existe tambem uma struct Lista_Texto, para armazenar uma lista de palavras (no caso, uma lista de Texto, e cada texto corresponde ao uma linha)
Abaixo temos um diagrama das structs e ponteiros

        Letra
 ┏━━━━━━━━┻━━━━━━━━┓
 ┃ ┏━━━━━━┳━━━━━━━┓┃  ┏━━━━━━┳━━━━━━━┓   ┏━━━━━━┳━━━━━━━┓   ┏━━━━━━┳━━━━━━━┓    ┏━━━━━━┳━━━━━━━━┓  
   ┃ char ┃ *prox ┃ ➔ ┃ char ┃ *prox ┃ ➔ ┃ char ┃ *prox ┃ ➔ ┃ char ┃ *prox ┃  ➔ ┃ char ┃ *➔ NULL┃
   ┗━━━━━━┻━━━━━━━┛   ┗━━━━━━┻━━━━━━━┛   ┗━━━━━━┻━━━━━━━┛   ┗━━━━━━┻━━━━━━━┛    ┗━━━━━━┻━━━━━━━━┛
       ▲                                                        ▲
       ┃        ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
       ┃        ┃
  ┏━━━━━━━━━┳━━━━━━━┳━━━━━━━━┓   ┏━━━━━━━━━┳━━━━━━━┳━━━━━━━━┓   ┏━━━━━━━━━┳━━━━━━━┳━━━━━━━━┓
  ┃ *Inicio ┃ *fim  ┃ *prox  ┃ ➔ ┃ *Inicio ┃ *fim  ┃ *prox  ┃ ➔ ┃ *Inicio ┃ *fim  ┃ *➔ NULL┃
  ┗━━━━━━━━━┻━━━━━━━┻━━━━━━━━┛   ┗━━━━━━━━━┻━━━━━━━┻━━━━━━━━┛   ┗━━━━━━━━━┻━━━━━━━┻━━━━━━━━┛
 ┗━━━━━━━━━━━━━━┳━━━━━━━━━━━━━┛                                        ▲
       ▲      Texto                                                    ┃
       ┃                                                               ┃
       ┃        ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
       ┃        ┃
  ┏━━━━━━━━━┳━━━━━━━┓   
  ┃ *Inicio ┃ *fim  ┃ 
  ┗━━━━━━━━━┻━━━━━━━┛ 
 ┗━━━━━━━━┳━━━━━━━━━━┛
     Lista_Texto

A rosoluão segue então na seguinte ordem:

1. Le o arquivo texto e gera uma lista encadeada de *Letras* retornando um *Texto* que contem o inicio e fim da lista de Letras que possui todos os caracteres do arquivo.
Interpretação da Primeira linha da mensagem
2. Realiza um split no *Texto* até a ocorrencia do primeiro caractere '\n', retorna uma *Lista_Texto*, contento na primeira posicão a primeira linha do protocolo e na segunda posição o resto da mensagem (cabeçalho e corpo)
3. Prga-se o *Texto* da primeira posição da Lista_Texto e faço o split ate a correncia do primeiro ' '(espaço). Na primeira __posição da lista__, tenha a primeira __posição da linha__ (Versão ou Metodo) e na segunda posição da lista, o resto da linha. Aqui ja faço uma strncmp, para comparar se os 4 primeiros caracteres dessa posição é 'HTTP', se sim, essa mensagem é uma resposta.
4. Em seguida realiza mais um split na primeira ocorrencia do ' ', tendo agora na primeira posição da Lista_Texto retornada codigo_do_status/url e na segunda posição descricao_status/versão.
5. Carrega-se essas informações na struct Http.
Interpretação do Cabeçalho
6. Essa parte é um loop, onde se processa linha a linha do cabecalho enquando o tamanho da lista Lista_Texto for menos de 1. 
    1. Realização do Split na primeira ocorrencia de '\n' e pega o Texto da primeira posição.
    2. Realiza o split na primeira ocorrencia de ':''
    3. Na primeira posição da Lista_Texto esta o nome do atributo do cabecalho
    4. Na segunda posição da Lista_Texto, a partir do segundo caractere(tem que pular o espaço inicial) esta o valor do atributo.
    5. Esses dados salvam em uma struct Cabecalho e adiciona na Lista_Cabecalho.
7. Ao fim do condição o Texto que sobra é o *corpo* da requisição
8. Apresenta na tela os dados.

#Detalhamento sobre a Resolução em Python

A ideia por traz da resolução em python, é a mesma da que foi utilizada em C, contudo o python abstrai toda a implementação da manipulação das listas, facilitando muito o processamento.

Execicio elaborado por: Rafael Silva Cunha
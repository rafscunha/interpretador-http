import sys

class HttpProtocol:
    tipo : str 
    metodo : str 
    url : str 
    versao : str 
    status_code : str 
    status_name : str 
    corpo : str 
    cabecalho: dict

def show_header(protocolo):
    print("----------     INTERPRETADOR PROTOCOLO HTTP    ----------")
    print("Tipo do protocolo: {}".format(protocolo.tipo))
    if protocolo.tipo == "Requisição":
        print("Método: {}\nURL: {}\nVersão: {}".format(protocolo.metodo, protocolo.url, protocolo.versao))
    else:
        print("Versão: {}\nStatus: {} - {}".format(protocolo.versao, protocolo.status_code, protocolo.status_name))

def show_cabecalho(protocolo):
    print("\n--------------------     CABEÇALHO    -------------------")
    for atributo_cabecalho in protocolo.cabecalho.keys():
        print("{} = {}".format(atributo_cabecalho, protocolo.cabecalho.get(atributo_cabecalho)))

def show_corpo(protocolo):
    if protocolo.corpo != '':
        print("\n----------------------     CORPO    ---------------------")
        print(protocolo.corpo)
    else:
        print("\n\n** Não existe corpo na mensagem **")

def carrega_header(protocolo, linha_header):
    lista_das_posicoes = linha_header.split(' ',2)
    if lista_das_posicoes[0] in ['GET', 'POST', 'HEAD', 'DELETE', 'PUT', 'OPTIONS', 'PATCH']:
        protocolo.tipo = "Requisição"
        protocolo.metodo = lista_das_posicoes[0]
        protocolo.url = lista_das_posicoes[1]
        protocolo.versao = lista_das_posicoes[2]
    else:
        protocolo.tipo = "Resposta"
        protocolo.versao = lista_das_posicoes[0]
        protocolo.status_code = lista_das_posicoes[1]
        protocolo.status_name = lista_das_posicoes[2]
    return protocolo
    
def carrega_cabecalho(protocolo, linha_cabecalho):
    protocolo.cabecalho = {}
    flag_run = True
    while flag_run:
        lista_linhas = linha_cabecalho.split('\n',1)
        vetor_linha = lista_linhas[0].split(': ')
        if len(vetor_linha) == 2:
            protocolo.cabecalho.update({vetor_linha[0]:vetor_linha[1]})
            linha_cabecalho = lista_linhas[1]
        else:
            try:
                protocolo.corpo = linha_cabecalho.split('\n',1)[1]
            except IndexError:
                protocolo.corpo = ''
            flag_run = False
    return protocolo

def main():
    protocolo = HttpProtocol()
    file = open(sys.argv[1], 'r').read()
    vetor_das_linhas = file.split('\n', 1)
    linha_header = vetor_das_linhas[0]
    carrega_header(protocolo,linha_header)
    carrega_cabecalho(protocolo,vetor_das_linhas[1])
    show_header(protocolo)
    show_cabecalho(protocolo)
    show_corpo(protocolo)

main()
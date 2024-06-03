#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <set>

#define RESET "\033[0m"
#define VERMELHO "\033[31m"
#define VERDE "\033[32m"
#define AMARELO "\033[33m"



class transicao{
    public:
        unsigned int currentState; 
        char currentSymbol;
        char writeSymbol;
        char direction; 
        int nextState; // Caso -1 == 'halt-accept'

    void mostraTransicao() const {
        std::cout << "Current State: " << currentState << "\n";
        std::cout << "Current Symbol: " << currentSymbol << "\n";
        std::cout << "Write Symbol: " << writeSymbol << "\n";
        std::cout << "Direction: " << direction << "\n";
        std::cout << "Next State: ";
        if (nextState == -1) {
            std::cout << "'halt-accept'\n";
        } else {
            std::cout << nextState << "\n";
        }
    }

};

class maquina{
    public:
        std::set<unsigned int> estados;
        std::set<char> alfabetoFita;
        std::set<char> alfabetoMaquina;
        std::set<transicao> transicoes;
};


void duplamenteInfinitaToSipser(std::vector<transicao>);
void sipserToDuplamenteInfinita(std::vector<transicao>);
void mostraTransicoes(std::vector<transicao> transicoes);
transicao criaTransicao(unsigned int, char, char, char, unsigned int );
void escreverArquivo(std::vector<transicao> , char );

int main(int argc, char** argv){

    std::vector<transicao> transicoes;

    if(argc == 1){
        std::cout << VERMELHO << "ERRO: "<< RESET << "Nenhum arquivo de entrada encontrado\n";
        return 1;
    }

    if(argc > 2){
        std::cout << VERMELHO << "ERRO: "<< RESET << "Argumentos desconhecidos:\n";
        for(int i = 2; i < argc; i++){
            std:: cout << VERMELHO << argv[i] << std::endl << RESET;
        }
        return 1;
    }

    std::string nomeArquivo(argv[1]);
    if((nomeArquivo.substr(nomeArquivo.find_last_of(".") + 1) != "txt") && (nomeArquivo.substr(nomeArquivo.find_last_of(".") + 1) != "in")) {
        std::cout << VERMELHO << "ERRO: "<< RESET << "Arquivo de entrada nao e do tipo '.txt'\n";
        return 1;
    }

    std::ifstream arquivo(argv[1]);
    std::string linha,tipo;
    std::getline(arquivo, tipo);
    maquina maq;
    if (arquivo.is_open()) { 
        unsigned int currentState; 
        char currentSymbol;
        char writeSymbol;
        char direction; 
        std::string nextState;
        transicao novaTransicao; 
        while (arquivo >> currentState >> currentSymbol >> writeSymbol >> direction >> nextState) { 
            transicao novaTransicao; 
            novaTransicao.currentState = currentState;
            novaTransicao.currentSymbol = currentSymbol;
            novaTransicao.writeSymbol = writeSymbol;
            novaTransicao.direction = direction;
            if(nextState == "halt-accept"){
                novaTransicao.nextState = -1;
            }else{
                novaTransicao.nextState = std::stoi(nextState);
            }
            transicoes.push_back(novaTransicao);
        }

    } else {
        std::cout << VERMELHO << "ERRO: " << RESET << "Arquivo nao encontrado\n";
        return 1;
    }

    for(int i = 0; i < transicoes.size(); i++){
        maq.alfabetoMaquina.insert(transicoes[i].currentSymbol);
        maq.alfabetoMaquina.insert(transicoes[i].writeSymbol);
    }

    if(tipo ==  ";S"){      // modelo de Sipser (fita com início à esquerda)
        std::cout << "Sipser\n";
        sipserToDuplamenteInfinita(transicoes);
        // Criar função sipser (converte fita com inicio à esquerda para fita duplamente infinita)
    }else if(tipo == ";I"){    //um programa para o modelo de fita duplamente infinita.
        std::cout << "duplamenteInfinita\n";
        duplamenteInfinitaToSipser(transicoes);
        // Criar função duplo infinito (converte fitaDuplamente infinita para fita com incio a esquerda)
    }else{
        std::cout << VERMELHO << "ERRO: "<< RESET << "Tipo do modelo nao especificado\n";
        return 1;
    }

    return 0;
}


void sipserToDuplamenteInfinita(std::vector<transicao> transicoes){

    // Soma dois a todos os estados criando espaco para marcaro inicio da fita
    for(int i = 0; i < transicoes.size(); i++){
        transicoes[i].currentState += 2;
        if(transicoes[i].nextState != -1){
            transicoes[i].nextState += 2;
        }
        
    }

    // Verifica se o cabecote da fita bateu na posicao mais a esquerda
    bool estadoJaCriado = false;
    int tamanhoInicial = transicoes.size();
    std::vector<int>transicoesCriadas;
    for(int i = 0; i < tamanhoInicial; i++){
        
        int proximoEstadoAntigo = transicoes[i].nextState;
        for(int j = 0; j < transicoesCriadas.size(); j++){
            if(transicoesCriadas[j] == proximoEstadoAntigo){
                estadoJaCriado = true;
                break;
            }
        }
        if(transicoes[i].direction == 'l' && proximoEstadoAntigo != -1 && !estadoJaCriado){
            transicoesCriadas.push_back(proximoEstadoAntigo);
            transicoes.push_back(criaTransicao(proximoEstadoAntigo,'#','#','r',proximoEstadoAntigo));
        }
        estadoJaCriado = false;
    }

    // Marcar inicio da fita
    transicoes.push_back(criaTransicao(0,'*','*','l',1));
    transicoes.push_back(criaTransicao(1,'_','#','r',2));

    // Escreve arquivo de saida
    escreverArquivo(transicoes,'I');
    return;
}


void duplamenteInfinitaToSipser(std::vector<transicao> transicoes){
    //mostraTransicoes(transicoes);

    // Soma 16 ao valor de todos os estados criando espaco para marca comeco e final da fita
    for(int i = 0; i < transicoes.size(); i++){
        transicoes[i].currentState += 16;
        if(transicoes[i].nextState != -1){
            transicoes[i].nextState += 16;
        }
        
    }


    int maiorEstado{0};
    for(int i = 0; i < transicoes.size(); i++){
        if(maiorEstado < transicoes[i].currentState){
            maiorEstado = transicoes[i].currentState;
        }
    }
    maiorEstado++;

    std::set<char> alfabetoFita;
    for(int i = 0; i < transicoes.size();i++){
        alfabetoFita.insert(transicoes[i].currentSymbol);
        alfabetoFita.insert(transicoes[i].writeSymbol);
    };;;;
    alfabetoFita.insert('_');

    
    bool estadoJaCriado = false;
    int tamanhoInicial = transicoes.size();
    std::vector<int>transicoesCriadas;

    //FIXME:
    // Tratar quando a bate no final
    for(int i = 0; i < tamanhoInicial;i++){

        for(int j = 0; j < transicoesCriadas.size();j++){
            if(transicoesCriadas[j] == transicoes[i].nextState){
                estadoJaCriado = true;
                //break;
            }
        }
        if(transicoes[i].direction == 'r' && transicoes[i].nextState != -1 && !estadoJaCriado){
            transicoes.push_back(criaTransicao(transicoes[i].nextState,'%','_','r',maiorEstado));
            transicoes.push_back(criaTransicao(maiorEstado,'_','%','l',transicoes[i].nextState));
            maiorEstado++;
            transicoesCriadas.push_back(transicoes[i].nextState);
        }
           estadoJaCriado = false;
    }


    transicoesCriadas.clear();

    for(int i = 0; i < transicoes.size(); i++){
        if(maiorEstado < transicoes[i].currentState){
            maiorEstado = transicoes[i].currentState;
        }
    }
    maiorEstado++;
    
    // FIXME: 
    // Tratar quando bate no inicio
    tamanhoInicial = transicoes.size();
    for(int i = 0; i < tamanhoInicial;i++){

        for(int j = 0; j < transicoesCriadas.size();j++){
            if(transicoesCriadas[j] == transicoes[i].nextState){
                estadoJaCriado = true;
            }
        }

        if(transicoes[i].direction == 'l' && transicoes[i].nextState != -1 && !estadoJaCriado){
            //Parte 1
            transicoes.push_back(criaTransicao(transicoes[i].nextState,'#','#','r',maiorEstado));
            transicoes.push_back(criaTransicao(maiorEstado,'*','*','r',maiorEstado));
            transicoes.push_back(criaTransicao(maiorEstado,'%','%','l',maiorEstado+1));
            int estadoImportante = maiorEstado + 1;
            int casoVazio = maiorEstado;
            //Parte 2
            int count = 1;
            int criadosPeloAlfabeto;
            for (int simbolo : alfabetoFita) {
                transicoes.push_back(criaTransicao(estadoImportante,simbolo,'$','r',maiorEstado+count));
                count++;
            }
            int ultimoEstadoCriado = maiorEstado + count;
            count = 1;
            //Parte 3
            for(int simbolo : alfabetoFita) {
                transicoes.push_back(criaTransicao(maiorEstado+count,'*',simbolo,'l',ultimoEstadoCriado + 1));
                transicoes.push_back(criaTransicao(maiorEstado+count,'%',simbolo,'r',ultimoEstadoCriado + 2));
                count++;
            }
             //Parte 4
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 1,'*','*','l',ultimoEstadoCriado + 1));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 1,'$','$','l',ultimoEstadoCriado + 3));

            //Parte 5
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 2,'_','%','l',ultimoEstadoCriado + 2));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 2,'*','*','l',ultimoEstadoCriado + 2));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 2,'$','$','r',ultimoEstadoCriado + 1));

            //Parte 6
            count = 1;
            for(int simbolo : alfabetoFita) {
                transicoes.push_back(criaTransicao(ultimoEstadoCriado + 3,simbolo,simbolo,'r',ultimoEstadoCriado + 4));
                count++;
            }
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 3,'$','$','r',ultimoEstadoCriado + 5));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 3,'#','#','r',ultimoEstadoCriado + 6));

            //Parte 7
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 4,'$','$','l',estadoImportante));

            // Parte 8
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 5,'*','*','r',ultimoEstadoCriado + 5));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 5,'_','#','l',ultimoEstadoCriado + 6));

            // Parte 9
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 6,'*','*','l',ultimoEstadoCriado + 6));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 6,'#','#','r',ultimoEstadoCriado + 7));

            //Parte 10
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 6,'$','_','l',ultimoEstadoCriado + 8));
            transicoes.push_back(criaTransicao(ultimoEstadoCriado + 8,'#','#','r',transicoes[i].nextState));

            maiorEstado = ultimoEstadoCriado + 9;
            transicoesCriadas.push_back(transicoes[i].nextState);
            
        }
        estadoJaCriado = false;
    }
    for(int i = 0; i < transicoesCriadas.size(); i++){
        
    }

 /*

;Parte 1  
8 # # r 9
9 * * r 9
9 % % l 10
Parte 2
10 1 $ r 11
10 0 $ r 12
10 _ $ r 13
;Parte 3
11 * 1 l 14
11 % 1 r 15
12 * 0 l 14
12 % 0 r 15
13 * _ l 14
13 % _ r 15
;Parte 4
14 * * l 14
14 $ $ l 16
; Parte 5
15 _ % l 15
15 * * l 15
15 $ $ r 14
;Parte 6
16 0 0 r 17
16 1 1 r 17
16 _ _ r 17
16 $ $ r 18
16 # # r 20
;Parte 7
17 $ $ l 10
;Parte 8
18 * * r 18
18 _ # l 19
; Parte 9
19 * * l 19
19 # # r 20
; Parte 10
20 $ _ l 21
21 # # r original
original _ 0 r 9


// Marcar comeco e fim da fita
0 * * r 0
0 _ _ l 1
1 1 # r 2
1 0 # r 3
2 * 1 l 4
3 * 0 l 4 
4 * * l 4
4 # # l 5
5 0 0 r 6
5 1 1 r 6
5 # # r 7
6 # # l 1
7 * * r 7
7 _ % l 8
8 * * l 8

*/   
    // Marca comeco e inicio da fita, e vai para o estado inicial original
    // Marca comeco da fita
    transicoes.push_back(criaTransicao(0, '*', '*', 'r', 0));
    transicoes.push_back(criaTransicao(0, '_', '_', 'l', 1));
    transicoes.push_back(criaTransicao(1, '1', '#', 'r', 2));
    transicoes.push_back(criaTransicao(1, '0', '#', 'r', 3));
    transicoes.push_back(criaTransicao(2, '*', '1', 'l', 4));
    transicoes.push_back(criaTransicao(3, '*', '0', 'l', 4));
    transicoes.push_back(criaTransicao(4, '*', '*', 'l', 4));
    transicoes.push_back(criaTransicao(4, '#', '#', 'l', 5));
    transicoes.push_back(criaTransicao(5, '0', '0', 'r', 6));
    transicoes.push_back(criaTransicao(5, '1', '1', 'r', 6));
    transicoes.push_back(criaTransicao(5, '#', '#', 'r', 7));
    transicoes.push_back(criaTransicao(5, '_', '_', 'r', 7));
    transicoes.push_back(criaTransicao(6, '#', '#', 'l', 1));
    //Marca final da fita
    transicoes.push_back(criaTransicao(7, '*', '*', 'r', 7));
    transicoes.push_back(criaTransicao(7, '_', '%', 'l', 8));
    transicoes.push_back(criaTransicao(8, '*', '*', 'l', 8));
    // Volta para o estado inicial com o cabecote da fita na posição correta
    transicoes.push_back(criaTransicao(8, '#', '#', 'r', 16));

    //Escreve arquivo de saida
    escreverArquivo(transicoes,'S');
    return;

}


void mostraTransicoes(std::vector<transicao> transicoes){
    std::cout<< "Transicoes: \n";
    for(int i = 0; i < transicoes.size(); i++){
        std::cout << "Estado Atual: " << transicoes[i].currentState << " "        
        << "Simbolo atual: " << transicoes[i].currentSymbol << " "
        << "Direcao: " << transicoes[i].direction << " ";
        if(transicoes[i].nextState == -1){
            std:: cout<< "Proximo estado: " << "halt-accept " << " ";
        }else{
        std:: cout << "Proximo estado: " << transicoes[i].nextState << " ";
        }
        std:: cout << "Simbolo escrito: " << transicoes[i].writeSymbol << "\n";
    }
}

void escreverArquivo(std::vector<transicao> transicoes, char tipo){
    std::ofstream arquivoSaida("saida.out");

     if (!arquivoSaida) {
       std::cout << VERMELHO << "ERRO: "<< RESET << "Problema ao escrever o arquivo\n";
        exit(-1); 
    }
    arquivoSaida << ";" << tipo << "\n\n";
    for(int i = 0; i < transicoes.size(); i++){
        arquivoSaida << transicoes[i].currentState << " "
        << transicoes[i].currentSymbol << " "
        << transicoes[i].writeSymbol << " "
        << transicoes[i].direction << " ";
        if(transicoes[i].nextState == -1){
           arquivoSaida << "halt-accept" << std::endl;

        }else{
            arquivoSaida << transicoes[i].nextState << std::endl;

        }
    }
    // <current state> <current symbol> <new symbol> <direction> <new state>

    arquivoSaida.close();
    return;
}


transicao criaTransicao(unsigned int currentState, char currentSymbol,char writeSymbol,char direction, unsigned int nextState){
    transicao novaTransicao;
    novaTransicao.currentState  = currentState;
    novaTransicao.currentSymbol = currentSymbol;
    novaTransicao.writeSymbol   = writeSymbol;
    novaTransicao.direction     = direction;
    novaTransicao.nextState     = nextState;
    
    return novaTransicao;
}






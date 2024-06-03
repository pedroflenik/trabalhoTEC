# trabalhoTEC

## Trabalho teoria da computação
O trabalho consite na criação de um "tradutor" que converte máquinas de Turing duplamente infinitas para máquinas de turing semi-infinita
e semi-infinita para duplamente infinita.

## Regras de entrada
- Todos os estados devem ser números inteiros positivos
- O estado final deve ser `halt-accept`
- Não é permitido o uso dos simbolos '#', '%', '$', e '_' no alfabeto da fita
- O programa não aceita comnetários no arquivo de entrada
- A primeira linha do arquivo de entrada deve especificar o tipo de máquina sendo `;S` para modelo de Sipser e `;I` modelo de fita duplamente infinita

## Como ultilizar
Assim que compilar o arquivo `trabalho.cpp` basta passar o arquivo de etrada na linha de comando.\
Por exemplo:\
`g++ trabalho.cpp -o trabalho`\
`./trabalho odd.txt`\
O programa vai produzir um arquivo chamado `saida.out`\
O conteúdo desse arquivo pode ser usado no simulador localizado [aqui](https://morphett.info/turing/turing.html)

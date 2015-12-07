# README #
** Visão Geral**
Simulação de um sistema com caches, barramento, memória e um componente conectado à memória que realiza operações de coerência de cache. Nesta implementação, o Diretório identifica se as linhas das caches encontram-se em estado “limpo” ou “sujo” e realiza ações dependendo da solicitação e estado das linhas.

### MEMBBROS DA EQUIPE ###
* Igor A. Brandão

### COMO COMPILAR ###
* Use o makefile digitando o comando **'make'** pelo terminal, após ter navegado para a pasta do projeto.

### COMO EXECUTAR O PROGRAMA ###
Para executar o Gremlins é necessário chamar o arquivo executável após compilar com o comando **'make'** pelo terminal,
assim:\n
* ./bin/aoc_cache\n

### LISTA DE CLASSES ###
As classes utilizadas pelo programa são as seguintes:

**cacheLine.h** => Provê a definição das linhas da cache.

**cache.h** => Implementa de fatos a estrutura da cache e suas funções de manipulação.

**directory.h** => Manipula os estados da linha da cache, assim como as operações envolvidas.

**directoryBlock.h** => Define os estados da linha da cache, podendo ser válido/inválido, limpo/sujo.
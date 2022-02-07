O ns-3 é um simulador de eventos discretos normalmente executado a partir da linha de comando. 
Ele é escrito diretamente em C++, eventos de simulação são simplesmente chamadas de função C++, 
organizadas por um agendador.

# Pre-requisitos

O ns-3 possui vários extensões opcionais, porém os recursos principais requerem apenas uma máquina
com o linux instalado, compilador C++ (g++ ou clang++) e Python (versão 3.6 ou superior). 
O Python é necessário para o sistema de compilação

# Como rodar o ns-3

No linux
Entre na pasta "ns-3.35"

Execute o seguinte comando para configurar o ns-3: 
$ ./waf configure --enable-examples --enable-tests

Execute o seguinte comando para buildar o ns-3: 
$ ./waf build

Execute o seguinte comando para rodar um exemplo que disponibilizamos no código que esta no git hub:
$  ./waf --run first

Mais informações podem ser encotradas na apostila de tutorial do ns-3:
https://www.nsnam.org/docs/release/3.35/tutorial/ns-3-tutorial.pdf
# Nosso Projeto

Nesse projeto estudamos e apresentamos os conceitos que regem uma simulação no NS3 juntamente com umexemplo nativo de sua biblioteca, durante a apresentação em sala. Nossa explicação sobre os
conceitos chave do NS3 também está contida nesse documento README. 
Adicionalmente, na semana que sucedeu a apresentação, conseguimos criar nossa própria simulação no
NS3! A partir de alterações do exemplo first.cc, botamos em prática os conceitos do NS3 criando uma
simulação que explora duas topologias, ponta-a-ponta e CSMA; o programa simula o envio de um pacote echo por parte de dois computadores na rede CSMA para um servidor ligado por uma conexão ponta-a-
ponta com essa subrede CSMA, que "ecoa" separadamente o pacote para cada um dos hosts que o enviou.
Esse diretório contém tanto o arquivo first.cc(Apresentado em aula) com suas respectivas leituras
pcap quanto o arquivo simEchoDuplo.cc(Nossa simulação), comentado para maior entendimento e com suas
respectivas leituras Pcap. Recomendamos a leitura dos comentários no código simEchoDuplo.cc para
entender as leituras pcap, que podem ser abertas com o Wireshark.

# Sobre o NS3

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

Para rodar a simulação que criamos, copie o arquivo simEchoDuplo.cc para a pasta ns-3.35/scratch 
Então volte para a pasta "ns-3.35" e execute o seguinte comando:
$  ./waf --run scratch/simEchoDuplo.cc

Mais informações podem ser encotradas na apostila de tutorial do ns-3:
https://www.nsnam.org/docs/release/3.35/tutorial/ns-3-tutorial.pdf

# Conceitos básicos de uma simulação

* __Nó__ 
Representam dispositivos computacionais na rede, geralmente representa hosts no caso da internet.
Todavia, também podem fazer o papel de roteadores e switches na simulação.
A classe mais básica é a classe *Node*.

* __Aplicação__
Assim como softwares(aplicações) rodam em computadores, as aplicações no NS3 rodam em cima dos
nós para criar determinados eventos durante a simulação. Nos exemplos que apresentamos, elas 
dirigem a forma como a simulação ocorre.
A classe mais básica é a classe *Application*

* __Canais__
Representam os canais de comunicação na rede, o meio pelo qual os dados são transmitidos, podem por exemplo representar um cabo ethernet ou um meio de transmissão wifi.
A classe mais básica é a classe *Channel*

* __Dispositivos de Rede__
Representa o dispositivo de rede que é acoplado no dispositivo computacional(nó) para que este possa
se comunicar com outros dispositos na rede através dos canais.
A classe mais básica é a classe *NetDevice*

* __Topology Helpers (Ajudantes de Topologia)__
Objetos que existem para facilitar operações necessárias para que a simulação ocorra, como
configurar canais e dispositivos de rede, definir endereços IP, entre outras. Os TopologyHelpers
realizam essas operações de forma encapsulada para o programador, que precisa apenas fornecer os
parâmetros que deseja para o objeto Helper. É uma alternativa à configurar cada objeto da simulação manualmente.
Dentre os diferentes tipos de Helpers, um exemplo é a classe *PointToPointHelper*.

* __Containers__
Armazenam ponteiros para os objetos criados na simulação, permitindo manipulá-los e referenciá-los de forma mais fácil.
Dentre os diferentes tipos de Container, um exemplo é o *NodeContainer*.

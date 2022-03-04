## Piratas-Server

Piratas Server é um chat básico de código aberto para comunicar com os membros e participantes do **Piratas Vix**, ele permite que vários clientes se conectem e proporciona aos usuários comodidade com sua privacidade e segurança em tempo real.
<br><br>

![alt text](docs/assets/img/diagrama-chat-socket-tcp.png)

<br>
Sua primeira versão é escrito em C/C++
<br><br>

#### Tutorial

Baixe ou clone o repositório `https://github.com/piratesvix/piratas-server`

<br>
<br>

###### Compilar projeto na linguagem C

```sh
gcc server.c -c <nome-do-executável>
```

<br>

###### Compilar projeto na linguagem C++

```sh
g++ server.c -o <nome-do-executável>
```

<br>
<br>

###### Compilar projeto com Make

Make é uma ferramenta que controla a geração de executáveis e outros arquivos. Com ele você pode compilar seu programa a partir de um arquivo chamado makefile.

No Windows é preciso instalar a ferraenta através do chocolatey `choco install make` e em seguida você  já pode construir seu executável rodando o comando:
 
```sh
make all
```


1- Clonar o repositório

2- Abrir o repositório no seu vs code

3- abrir o terminal git bash(no vs code)

4- dar os seguintes comandos:

mkdir build

cmake -G Ninja -S . -B build

caso a pasta build já exista voce exclui ela dando o comando rm -rf build e depois da os comando do passo 4.

5- conectar a placa ao pc no modo boot, e então apertar em run project(USB), da extensão do raspberry pi.

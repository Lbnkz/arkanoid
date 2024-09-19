# Usa uma imagem base do Ubuntu
FROM ubuntu:20.04

# Instala o build-essential para compilar C++
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Define o diretório de trabalho
WORKDIR /app

# Copia o código-fonte para o contêiner
COPY . .

# Compila o código C++
RUN g++ -o arkanoid main.cpp

# Comando para executar o jogo
CMD ["./arkanoid"]

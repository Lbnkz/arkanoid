#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int width = 40;    // Largura da tela do jogo
const int height = 20;   // Altura da tela do jogo
int score = 0;           // Pontuação do jogador

// Velocidades
int ballSpeed = 50;      // Velocidade da bola (milissegundos para mover)
int paddleSpeed = 2;     // Velocidade da raquete (incremento por movimento)

// Posições iniciais
int ballX = width / 2, ballY = height / 2; // Posição inicial da bola
int ballDirX = -1, ballDirY = -1;          // Direção inicial da bola
int paddleX = width / 2 - 4;               // Posição inicial da raquete
int paddleWidth = 8;                       // Largura da raquete

// Blocos
bool blocks[5][width]; // Array de blocos

// Estados do jogo
bool gameOver = false;   // Indica se o jogo terminou
bool win = false;        // Indica se o jogador venceu


// Função para inicializar os blocos
void iniciarBlocos() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < width; j++) {
            blocks[i][j] = true; // Todos os blocos estão ativos no início
        }
    }
}

// Função para reiniciar o jogo
void limpaJogo() {
    score = 0; // Reseta a pontuação
    ballX = width / 2;
    ballY = height / 2;
    ballDirX = -1;
    ballDirY = -1;
    paddleX = width / 2 - 4;
    gameOver = false;
    win = false;
    iniciarBlocos(); // Reinicia os blocos
}

// Função para desenhar o jogo na tela
void desenhaJogo() {
    //ALERTA: NÃO MODIFICAR O TRECHO DE CÓDIGO, A SEGUIR.
    //INICIO: COMANDOS PARA QUE O CURSOR NÃO FIQUE PISCANDO NA TELA
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    //FIM: COMANDOS PARA QUE O CURSOR NÃO FIQUE PISCANDO NA TELA

    //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INÍCIO DA TELA
    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    SetConsoleCursorPosition(out, coord); // Reposiciona o cursor
    //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INÍCIO DA TELA
    //ALERTA: NÃO MODIFICAR O TRECHO DE CÓDIGO, ACIMA.

    // Desenha os blocos
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < width; j++) {
            if (blocks[i][j])
                cout << "#";
            else
                cout << " ";
        }
        cout << endl;
    }

    // Desenha a parte inferior do jogo (onde a bola e a raquete estão)
    for (int i = 5; i < height - 1; i++) {
        for (int j = 0; j < width; j++) {
            if (i == ballY && j == ballX)
                cout << "O"; // Bola
            else
                cout << " ";
        }
        cout << endl;
    }

    // Desenha a raquete
    for (int i = 0; i < width; i++) {
        if (i >= paddleX && i < paddleX + paddleWidth)
            cout << "="; // Raquete
        else
            cout << " ";
    }
    cout << endl;

    // Desenha a borda inferior
    for (int i = 0; i < width; i++) cout << "-";
    cout << endl;

    // Mostra a pontuação
    cout << "Pontuação: " << score << endl;
}

// Função para mover a bola
void atualizaBola() {
    ballX += ballDirX;
    ballY += ballDirY;

    // Colisão com paredes laterais
    if (ballX <= 0 || ballX >= width - 1) {
        ballDirX = -ballDirX;
    }

    // Colisão com o topo da tela
    if (ballY <= 0) {
        ballDirY = -ballDirY;
    }

    // Colisão com a raquete
    if (ballY == height - 2 && ballX >= paddleX && ballX < paddleX + paddleWidth) {
        ballDirY = -ballDirY;
    }

    // Colisão com blocos
    if (ballY < 5 && blocks[ballY][ballX]) {
        blocks[ballY][ballX] = false; // Destrói o bloco
        ballDirY = -ballDirY;         // Inverte a direção da bola
        score += 10;                  // Adiciona pontos
    }

    // Verifica se a bola caiu abaixo da raquete
    if (ballY >= height - 1) {
        gameOver = true;
    }
}

// Função para mover a raquete
void atualizaRaquete() {
    if (_kbhit()) {
        char tecla = _getch();
        if ((tecla == 'a' || tecla == 75) && paddleX > 0) {
            paddleX -= paddleSpeed; // Move para a esquerda
            if (paddleX < 0) paddleX = 0; // Impede de ultrapassar a borda esquerda
        }
        if ((tecla == 'd' || tecla == 77) && paddleX + paddleWidth < width) {
            paddleX += paddleSpeed; // Move para a direita
            if (paddleX + paddleWidth > width) paddleX = width - paddleWidth; // Impede de ultrapassar a borda direita
        }
    }
}

// Função para exibir o menu principal
void mostraMenu() {
    system("cls"); // Limpa a tela
    cout << "==================== ARKANOID ====================" << endl;
    cout << "1. Iniciar Jogo" << endl;
    cout << "2. Instrucoes" << endl;
    cout << "3. Sobre" << endl;
    cout << "4. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

// Função para exibir instruções do jogo
void instrucoes() {
    system("cls"); // Limpa a tela
    cout << "Instrucoes:" << endl;
    cout << "1. Use as teclas esquerda e direita para mover a raquete." << endl;
    cout << "2. Destrua todos os blocos para vencer o jogo." << endl;
    cout << "3. Evite que a bola caia no chão." << endl;
    cout << "4. Cada bloco destruído vale 10 pontos." << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

// Função para exibir informações sobre o jogo e a equipe
void sobre() {
    system("cls"); // Limpa a tela
    cout << "Sobre o jogo:" << endl;
    cout << "Desenvolvido por: Equipe Lucas e Kaue" << endl;
    cout << "Versao: 1.0" << endl;
    cout << "Ano: 2024" << endl;
    cout << "- Lucas e Kaue: Implementacao da logica de colisao" << endl;
    cout << "- Kaue: Desenvolvimento da interface e menu" << endl;
    cout << "- Lucas: Implementacao do sistema de pontos" << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

//Função para exibir a tela de opção inválida
void opcaoInvalida() {
    system("cls"); // Limpa a tela
    cout << "Opcao invalida! Tente novamente." << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

// Função principal para controlar o jogo
void loopJogo() {
    iniciarBlocos();

    // Loop principal do jogo
    while (!gameOver && !win) {
        desenhaJogo();          // Renderiza o campo de jogo
        atualizaBola();    // Atualiza a posição da bola
        atualizaRaquete();  // Atualiza a posição da raquete

        // Verifica condição de vitória
        win = true;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < width; j++) {
                if (blocks[i][j]) {
                    win = false;
                    break;
                }
            }
            if (!win) break;
        }

        Sleep(ballSpeed); // Controla a velocidade do jogo
    }

    if (gameOver) {
        cout << "Game Over! Pontuacao final: " << score << endl;
    } else if (win) {
        cout << "Parabens! Voce venceu o jogo com " << score << " pontos!" << endl;
    }
}

// Função para perguntar se o jogador quer jogar novamente
void jogueDeNovo() {
    char choice;
    do {
        cout << "Deseja jogar novamente? (s/n): ";
        cin >> choice;
        if (choice == 's' || choice == 'S') {
            limpaJogo();
            loopJogo();
        } else if (choice == 'n' || choice == 'N') {
            cout << "Obrigado por jogar!" << endl;
            return;
        } else {
            cout << "Opcao invalida! Tente novamente." << endl;
        }
    } while (choice != 's' && choice != 'S' && choice != 'n' && choice != 'N');
}

int main() {
    srand(time(0)); // Inicializa o gerador de números aleatórios

    while (true) {
        mostraMenu();

        int opcao;
        cin >> opcao;

        switch (opcao) {
            case 1:
                limpaJogo();
                loopJogo();
                jogueDeNovo();
                break;
            case 2:
                instrucoes();
                break;
            case 3:
                sobre();
                break;
            case 4:
                cout << "Saindo do jogo..." << endl;
                return 0;
            default:
                opcaoInvalida();
                break;
        }
    }

    return 0;
}
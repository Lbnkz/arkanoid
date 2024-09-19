#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

const int width = 40;
const int height = 20;
const int marginTop = 2;
const int marginSide = 2;  // Margem lateral ajustada
int score = 0;

// Velocidades
int ballSpeed = 0;
int paddleSpeed = 2;

// Posições iniciais
int ballX = width / 2, ballY = height / 2; // Posição inicial da bola
int ballDirX = -1, ballDirY = -1;          // Direção inicial da bola
int paddleX = width / 2 - 4;               // Posição inicial da raquete
int paddleWidth = 8;                       // Largura da raquete

// Blocos
const int numBlocosLargura = (width - 3 * marginSide); // Ajustado para respeitar a margem lateral
const int numBlocosAltura = 5;
bool blocks[numBlocosAltura][numBlocosLargura];

// Estados do jogo
bool gameOver = false;   // Indica se o jogo terminou
bool win = false;        // Indica se o jogador venceu

// Função para inicializar os blocos
void iniciarBlocos() {
    for (int i = 0; i < numBlocosAltura; i++) {
        for (int j = 0; j < numBlocosLargura; j++) {
            blocks[i][j] = true;
        }
    }
}

// Função para reiniciar o jogo
void limpaJogo() {
    score = 0;
    ballX = width / 2;
    ballY = height / 2;
    ballDirX = -1;
    ballDirY = -1;
    paddleX = width / 2 - 4;
    gameOver = false;
    win = false;
    iniciarBlocos();
}

// Função para desenhar o jogo na tela
void desenhaJogo() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    COORD coord = {0, 0};
    SetConsoleCursorPosition(out, coord); // Reposiciona o cursor

    // Parede superior
    cout << string(width, '#') << endl;

    // Adiciona linhas em branco para espaçar os blocos do topo da tela
    for (int i = 0; i < marginTop; i++) {
        cout << "#" << string(width - 2, ' ') << "#" << endl;
    }

    // Desenha os blocos com margens laterais
    for (int i = 0; i < numBlocosAltura; i++) {
        cout << "#";  // Parede esquerda
        for (int j = 0; j < width - 2; j++) { // Preenche a largura entre as duas paredes
            if (j >= marginSide && j < marginSide + numBlocosLargura) {
                if (blocks[i][j - marginSide])
                    cout << "X";  // Bloco
                else
                    cout << " ";  // Espaço vazio
            } else {
                cout << " ";  // Área fora dos blocos
            }
        }
        cout << "#" << endl;  // Parede direita
    }

    // Desenha a parte inferior do jogo (onde a bola e a raquete estão)
    for (int i = 5; i < height - 1; i++) {
        cout << "#";  // Parede esquerda
        for (int j = 0; j < width - 2; j++) {
            if (i == ballY && j + 1 == ballX) // Ajustando a posição da bola
                cout << "O"; // Desenha a bola
            else if (j >= marginSide && j < width - marginSide - 1) {
                cout << " ";  // Espaço vazio
            } else {
                cout << " ";  // Área fora dos blocos
            }
        }
        cout << "#" << endl;  // Parede direita
    }

    // Desenha a raquete
    cout << "#";  // Parede esquerda
    for (int i = 0; i < width - 2; i++) {
        if (i >= paddleX && i < paddleX + paddleWidth)
            cout << "=";  // Raquete
        else
            cout << " ";
    }
    cout << "#" << endl;  // Parede direita

    // Desenha a borda inferior
    cout << string(width, '#') << endl;

    // Mostra a pontuação
    cout << "Pontuação: " << score << endl;
}

// Função para jogar a bolinha para uma direção aleatoria
int direcaoAleatoria() {
    return rand() % 2 == 0 ? 1 : -1;
}

void atualizaBola() {
    ballX += ballDirX;
    ballY += ballDirY;

    // Colisão com paredes laterais
    if (ballX <= marginSide || ballX >= width - marginSide - 1) {
        ballDirX = -ballDirX;
    }

    // Colisão com o topo da tela
    if (ballY <= marginTop) {
        ballDirY = -ballDirY;
    }

    // Colisão com a raquete
    if (ballY == height - 2 && ballX >= paddleX && ballX < paddleX + paddleWidth) {
        ballDirY = -ballDirY;
        ballDirX = direcaoAleatoria(); // Direção aleatória na colisão
    }

    // Colisão com blocos
    if (ballY >= marginTop && ballY < marginTop + numBlocosAltura && ballX >= marginSide && ballX < width - marginSide) {
        int blockRow = ballY - marginTop; // Linha do bloco com base na posição da bola
        int blockCol = ballX - marginSide - 1; // Coluna do bloco ajustada pela margem lateral

        if (blockRow >= 0 && blockRow < numBlocosAltura && blockCol >= 0 && blockCol < numBlocosLargura) {
            if (blocks[blockRow][blockCol]) {
                blocks[blockRow][blockCol] = false; // Destrói o bloco
                ballDirY = -ballDirY;            // Inverte a direção vertical da bola
                score += 10;                     // Adiciona pontos
                ballDirX = direcaoAleatoria();   // Define uma nova direção horizontal
            }
        }
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

//Função para exibir a tela de opção inválida
void opcaoInvalida() {
    system("cls");
    cout << "Opcao invalida! Tente novamente." << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

// Função para exibir o menu principal
void mostraMenu() {
    system("cls");
    cout << "=============== ARKANOID ===============" << endl;
    cout << "1. Iniciar Jogo" << endl;
    cout << "2. Instrucoes" << endl;
    cout << "3. Sobre" << endl;
    cout << "4. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

// Função para exibir o menu de dificuldade
void menuDificuldade() {
    system("cls");
    cout << "=============== ARKANOID ===============" << endl;
    cout << "Selecione a dificuldade:" << endl;
    cout << "1. Facil" << endl;
    cout << "2. Medio" << endl;
    cout << "3. Dificil" << endl;
    cout << "Escolha uma opcao: ";
    int opcao;
    cin >> opcao;
    switch (opcao) {
        case 1:
            ballSpeed = 100; // Velocidade da bola: 100 ms
            break;
        case 2:
            ballSpeed = 50; // Velocidade da bola: 50 ms
            break;
        case 3:
            ballSpeed = 25; // Velocidade da bola: 25 ms
            break;
        default:
            opcaoInvalida();
            break;
    }
}
// Função para exibir instruções do jogo
void instrucoes() {
    system("cls");
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
    system("cls");
    cout << "Sobre o jogo:" << endl;
    cout << "Desenvolvido por: Equipe Lucas e Kaue" << endl;
    cout << "Versao: 1.0" << endl;
    cout << "Ano: 2024" << endl;
    cout << "- Lucas e Kaue: Implementacao da logica de colisao" << endl;
    cout << "- Kaue: Desenvolvimento da interface e menu" << endl;
    cout << "- Kaue: Desenvolvimento dos modos de dificuladade do jogo" << endl;
    cout << "- Lucas: Implementacao do sistema de pontos" << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

// Função principal para controlar o jogo
void loopJogo() {
    limpaJogo();
    system("cls");
    while (!gameOver && !win) {
        desenhaJogo();
        atualizaBola();
        atualizaRaquete();
        Sleep(ballSpeed); // Controle de velocidade
        win = true;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < width - 2 * marginSide; j++) {
                if (blocks[i][j]) {
                    win = false;
                    break;
                }
            }
            if (!win) break;
        }
    }

    // Exibe mensagem de fim de jogo
    system("cls");
    if (win) {
        cout << "Parabens! Voce venceu o jogo!" << endl;
    } else {
        cout << "Game Over!" << endl;
    }
    cout << "Sua pontuacao final foi: " << score << endl;
}

// Função para perguntar se o jogador quer jogar novamente
void jogueDeNovo() {
    char choice;
    do {
        cout << "Deseja jogar novamente? (s/n): ";
        cin >> choice;
        if (choice == 's' || choice == 'S') {
            limpaJogo();
            menuDificuldade();
            loopJogo();
        } else if (choice == 'n' || choice == 'N') {
            cout << "Obrigado por jogar!" << endl;
            return;
        } else {
            cout << "Opcao invalida! Tente novamente." << endl;
        }
    } while (choice != 'n' && choice != 'N');
}

int main() {
    srand(time(0));

    while (true) {
        system("chcp 65001");
        mostraMenu();

        int opcao;
        if (!(cin >> opcao)) {
            // Se cin falhar, limpa o erro e descarta a entrada incorreta
            cin.clear(); // Limpa o estado de erro
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta a linha
            opcaoInvalida();
            continue; // Retorna ao início do loop para mostrar o menu novamente
        }
        switch (opcao) {
            case 1:
                limpaJogo();
                menuDificuldade();
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

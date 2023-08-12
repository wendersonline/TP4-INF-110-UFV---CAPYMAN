#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   Wenderson Rodrigues Lopes Matrícula: 112707

const int SIZE = 50;      // Tamanho de cada célula do mapa
const char vazio = ' ';

char mapa[19][20] = {     // Mapa do jogo
  "1111111111111111111",
  "1000000001000000001",
  "1011011101011101101",
  "1000000001000000001",
  "1011010111110101101",
  "1000010001000100001",
  "1111011101011101111",
  "3331010000000101333",
  "1111010133310101111",
  "0000000133310000000",
  "1111010111110101111",
  "3331010000000101333",
  "1111011101011101111",
  "1000010001000100001",
  "1011010111110101101",
  "1000000000000000001",
  "1011011101011101101",
  "1000000001000000001",
  "1111111111111111111",
};

int posx = 17;       // posicao do PacMan
int posy = 13;

bool cima = false;  // direcao de movimento do PacMan
bool baixo = false;
bool esq = false;
bool dir = false;

bool up = true;    // direcao de movimento do fantasma 1
bool down = false;
bool left = false;
bool right = false;

bool up2 = true;   // direcao de movimento do fantasma 2
bool down2 = false;
bool left2 = false;
bool right2 = false;

bool up3 = true;   // direcao de movimento do fantasma 3
bool down3 = false;
bool left3 = false;
bool right3 = false;

bool up4 = true;   // direcao de movimento do fantasma 4
bool down4 = false;
bool left4 = false;
bool right4 = false;

int count = 0;
int cont = 0;

bool follow = false;


bool checkCollision(int ghostX, int ghostY, int pacmanX, int pacmanY) {
    return (ghostX == pacmanX && ghostY == pacmanY);
}

float distance(int posxf, int posyf, int posxPacMan, int posyPacMan) {
    int distanciaX = posxf - posx;
    int distanciaY = posyf - posy;

    return sqrt((distanciaX * distanciaX) + (distanciaY * distanciaY));

}

bool verificaProximaPosicao(int posx, int posy, int direcao) {
    bool isValida = false;

    switch (direcao) {
    case 0:
        if (mapa[posx - 1][posy] != '1') {
            isValida = true;
        }
        break;
    case 1:
        if (mapa[posx + 1][posy] != '1') {
            isValida = true;
        }
        break;
    case 2:
        if (mapa[posx][posy - 1] != '1') {
            isValida = true;
        }
        break;
    case 3:
        if (mapa[posx][posy + 1] != '1') {
            isValida = true;
        }
        break;
    }

    return isValida;
}


void updateGhostMovement(int& posxf, int& posyf, bool& up, bool& down, bool& left, bool& right, int& posx, int& posy, bool& seguindo) {
    std::vector<int> direcoesPossiveis;

    direcoesPossiveis.clear();    // Zera as direções possíveis

    if (seguindo) {
        if (verificaProximaPosicao(posxf, posyf, 0) && !down) {           //Verifica e retorna o caso das direções possíveis
            direcoesPossiveis.push_back(0);
        }
        if (verificaProximaPosicao(posxf, posyf, 1) && !up) {
            direcoesPossiveis.push_back(1);
        }
        if (verificaProximaPosicao(posxf, posyf, 2) && !right) {
            direcoesPossiveis.push_back(2);
        }
        if (verificaProximaPosicao(posxf, posyf, 3) && !left) {
            direcoesPossiveis.push_back(3);
        }

        if (!direcoesPossiveis.empty()) {
            // Verifica qual direção leva mais perto do Pac-Man
            int melhorDirecao = direcoesPossiveis[0];
            float menorDistancia = distance(posxf, posyf, posx, posy);

            for (int i = 1; i < direcoesPossiveis.size(); i++) {
                int direcao = direcoesPossiveis[i];
                int proxPosxf = posxf;
                int proxPosyf = posyf;

                switch (direcao) {
                case 0: // Move para cima
                    proxPosxf--;
                    break;
                case 1: // Move para baixo
                    proxPosxf++;
                    break;
                case 2: // Move para esquerda
                    proxPosyf--;
                    break;
                case 3: // Move para direita
                    proxPosyf++;
                    break;
                }

                float distancia = distance(proxPosxf, proxPosyf, posx, posy);

                if (distancia < menorDistancia) {
                    melhorDirecao = direcao;
                    menorDistancia = distancia;
                }
            }

            // Atualiza a direção do fantasma para a melhor direção
            switch (melhorDirecao) {
            case 0: // Move para cima
                up = true;
                down = left = right = false;
                break;
            case 1: // Move para baixo
                down = true;
                up = left = right = false;
                break;
            case 2: // Move para esquerda
                left = true;
                up = down = right = false;
                break;
            case 3: // Move para direita
                right = true;
                down = up = left = false;
                break;
            }
        }

        // Corrige a direção caso passe pelo túnel
        if (posyf < 0) {
            posyf = 18;
            left = true;
            down = up = right = false;
        }
        else if (posyf > 18) {
            posyf = 0;
            right = true;
            down = up = left = false;
        }

        // Movimenta o fantasma na direção atual
        if (up && mapa[posxf - 1][posyf] != '1') {
            posxf--;
        }
        if (down && mapa[posxf + 1][posyf] != '1') {
            posxf++;
        }
        if (left && mapa[posxf][posyf - 1] != '1') {
            posyf--;
        }
        if (right && mapa[posxf][posyf + 1] != '1') {
            posyf++;
        }
    }

    if (!seguindo) {
        if (verificaProximaPosicao(posxf, posyf, 0) && !down) {           //Verifica e retorna o caso das direções possíveis
            direcoesPossiveis.push_back(0);
        }
        if (verificaProximaPosicao(posxf, posyf, 1) && !up) {
            direcoesPossiveis.push_back(1);
        }
        if (verificaProximaPosicao(posxf, posyf, 2) && !right) {
            direcoesPossiveis.push_back(2);
        }
        if (verificaProximaPosicao(posxf, posyf, 3) && !left) {
            direcoesPossiveis.push_back(3);
        }


        if (up && !verificaProximaPosicao(posxf, posyf, 0) ||
            down && !verificaProximaPosicao(posxf, posyf, 1) ||         // Seleciona as direções possíveis quando true
            left && !verificaProximaPosicao(posxf, posyf, 2) ||
            right && !verificaProximaPosicao(posxf, posyf, 3) || direcoesPossiveis.size() >= 2) {

            if (!direcoesPossiveis.empty()) {
                int randomDirectionIndex = rand() % direcoesPossiveis.size();
                int randomDirection = direcoesPossiveis[randomDirectionIndex];
                switch (randomDirection) {                // Modifica a direção aleatoriamente
                case 0: // Move para cima
                    if (verificaProximaPosicao(posxf, posyf, 0)) {
                        up = true;
                        down = right = left = false;
                    }
                    break;
                case 1: // Move para baixo
                    if (verificaProximaPosicao(posxf, posyf, 1)) {
                        down = true;
                        up = right = left = false;
                    }
                    break;
                case 2: // Move para esquerda
                    if (verificaProximaPosicao(posxf, posyf, 2)) {
                        left = true;
                        down = right = up = false;
                    }
                    break;
                case 3: // Move para direita
                    if (verificaProximaPosicao(posxf, posyf, 3)) {
                        right = true;
                        down = up = left = false;
                    }
                    break;
                }
            }

            else {             // Se não modifica, deixa na direção que está.
                if (up) {
                    down = true;
                    up = right = left = false;
                }
                else if (down) {
                    up = true;
                    down = right = left = false;
                }
                else if (left) {
                    right = true;
                    down = up = left = false;
                }
                else if (right) {
                    left = true;
                    down = up = right = false;
                }
            }

            // Corrige a direção caso passe pelo túnel
            if (posyf < 0) {
                posyf = 18;
                left = true;
                down = up = right = false;
            }
            else if (posyf > 18) {
                posyf = 0;
                right = true;
                down = up = left = false;
            }
        }





        // Direções

        if (up) {
            posxf--;
        }
        if (down) {
            posxf++;
        }

        if (left) {
            posyf--;
        }

        if (right) {
            posyf++;
        }

    }
}


int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(950, 1000), "Pac-Man");


    // shape da parede
    sf::Texture pilastra;
    if (!pilastra.loadFromFile("images/pilastra.png")) {
        std::cout << "Erro lendo imagem pilastra.png\n";
        return 0;
    }
    sf::Sprite pilas;
    pilas.setTexture(pilastra);
    

    // shape do portão da toca dos fantasmas
    sf::RectangleShape line(sf::Vector2f(1 * SIZE, 0.1 * SIZE));
    line.setFillColor(sf::Color(169, 169, 169));


    // sprite da Capivara
    sf::Texture texture;
    if (!texture.loadFromFile("images/capy.png")) {
        std::cout << "Erro lendo imagem pacman.png\n";
        return 0;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);



    // Pontuação
    int score = 0;


    // sprite dos Jacarés
    sf::Texture text1;
    if (!text1.loadFromFile("images/jacare1.png")) {
        std::cout << "Erro lendo imagem fantasma.png\n";
        return 0;
    }
    sf::Sprite jacare;
    jacare.setTexture(text1);
    int posxf = 7;
    int posyf = 9;


    sf::Texture text2;
    if (!text2.loadFromFile("images/jacare2.png")) {
        std::cout << "Erro lendo imagem fantasma.png\n";
        return 0;
    }
    sf::Sprite jacare2;
    jacare2.setTexture(text2);

    int posxf2 = 9;
    int posyf2 = 9;


    sf::Texture text3;
    if (!text3.loadFromFile("images/jacare3.png")) {
        std::cout << "Erro lendo imagem fantasma.png\n";
        return 0;
    }
    sf::Sprite jacare3;
    jacare3.setTexture(text3);

    int posxf3 = 9;
    int posyf3 = 10;


    sf::Texture text4;
    if (!text4.loadFromFile("images/jacare4.png")) {
        std::cout << "Erro lendo imagem fantasma.png\n";
        return 0;
    }
    sf::Sprite jacare4;
    jacare4.setTexture(text4);

    int posxf4 = 9;
    int posyf4 = 8;



    // sprite pilulas
    sf::CircleShape pil(5);
    pil.setFillColor(sf::Color(255, 255, 255));
    pil.setTexture(nullptr);



    // cria um relogio para medir o tempo do PacMan
    sf::Clock clock;
    const sf::Time intervalo = sf::seconds(0.15);



    sf::Clock timer;
    const sf::Time intervalo2 = sf::seconds(0.30);

    sf::Clock followTimer;
    const sf::Time followDuration = sf::seconds(15.f);


    sf::Music music;
    if (!music.openFromFile("audios/start.wav")) {
        return -1;
    }
    music.play();

    sf::Music musicDie;
    if (!musicDie.openFromFile("audios/die.wav")) {
        return -1;
    }

    sf::Music musicMove;
    if (!musicMove.openFromFile("audios/move.wav")) {
        return -1;
    }

    sf::Music musicSiren;
    if (!musicSiren.openFromFile("audios/siren.wav")) {
        return -1;
    }

    sf::Font font;
    if (!font.loadFromFile("font/Minecraft.ttf")) {
        std::cout << "Erro ao carregar a fonte arial.ttf\n";
        return 0;

    }


    bool start = false;
    bool gameOver = false;
    bool win = false;
    bool restart = false;


    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();

            if (!start && music.getStatus() == sf::SoundSource::Stopped) {
                start = true;
            }

            if (start) {
                music.stop();
            }

            // tecla pressionada
            if (!gameOver && event.type == sf::Event::KeyPressed && start && !win) {
                if (event.key.code == sf::Keyboard::Left && mapa[posx][posy - 1] != '1' && mapa[posx][posy - 1] != '3') {
                    esq = true;     // left key: PacMan passa a mover para esquerda
                    dir = cima = baixo = false;
                }
                else if (event.key.code == sf::Keyboard::Right && mapa[posx][posy + 1] != '1' && mapa[posx][posy + 1] != '3') {
                    dir = true;    // right key: PacMan passa a mover para direita
                    esq = cima = baixo = false;
                }
                else if (event.key.code == sf::Keyboard::Up && mapa[posx - 1][posy] != '1' && mapa[posx - 1][posy] != '3') {
                    cima = true;  // up key: PacMan passa a mover para cima
                    esq = dir = baixo = false;
                }
                else if (event.key.code == sf::Keyboard::Down && mapa[posx + 1][posy] != '1' && mapa[posx + 1][posy] != '3') {
                    baixo = true; // down key:PacMan passa a mover para baixo
                    esq = dir = cima = false;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            restart = true;
        }

        // Muda a posição do PacMan a cada 0.2 segundos
        if (!gameOver && clock.getElapsedTime() >= intervalo && !win) {    // tempo desde último restart > 0.2s?
            clock.restart();        // recomeça contagem do tempo

            if (cima && mapa[posx - 1][posy] != '1' && mapa[posx - 1][posy] != '3' && start) {       // muda a posição de acordo com booleano ativo
                posx--;
            }
            if (baixo && mapa[posx + 1][posy] != '1' && mapa[posx + 1][posy] != '3' && start) {
                posx++;
            }
            if (esq && mapa[posx][posy - 1] != '1' && mapa[posx][posy - 1] != '3' && start) {
                posy--;
                texture.loadFromFile("images/capyEsq.png");
            }
            if (dir && mapa[posx][posy + 1] != '1' && mapa[posx][posy + 1] != '3' && start) {
                posy++;
                texture.loadFromFile("images/capy.png");
            }
        }



        // Se posição horizontal for maior que o limite do mapa ele retorna na outra saida do mapa
        if (posy < 0) {
            posy = 18;
        }
        else if (posy > 18) {
            posy = 0;
        }

        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if (mapa[i][j] == '2') {
                    mapa[i][j] = '0';
                }
            }
        }



        // limpa a janela com a cor AZUL de fundo
        window.clear(sf::Color(121, 182, 232));



        // desenhar tudo aqui...

        // desenha paredes
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if (mapa[i][j] == '1') {
                    pilas.setPosition(j * SIZE, i * SIZE);
                    window.draw(pilas);
                }
                if (i == 8 && j == 8 || i == 9 && j == 8 || i == 10 && j == 8) {
                    line.setPosition(i * SIZE, j * SIZE);
                    window.draw(line);
                }
                // desenha pilulas
                else if (mapa[i][j] == '0') {
                    mapa[i][j] == '2';
                    pil.setPosition(j * SIZE + SIZE / 2, i * SIZE + SIZE / 2);
                    window.draw(pil);
                }
            }
        }

        // desenha PacMan
        sprite.setPosition(posy * SIZE, posx * SIZE);
        window.draw(sprite);


        // Posição fantasma

        if (followTimer.getElapsedTime() >= sf::seconds(20.0f)) {
            followTimer.restart();
            follow = false;
        }
        else if (followTimer.getElapsedTime() > sf::seconds(15.0f)) {
            follow = true;
        }

        if (followTimer.getElapsedTime() <= sf::seconds(7.0f)) {
            follow = false;
        }

        if (timer.getElapsedTime() >= intervalo2 && start && !gameOver && !win) {
            timer.restart();

            if (cont == 0 && !win && !gameOver) {
                musicSiren.play();
            }

            updateGhostMovement(posxf, posyf, up, down, left, right, posx, posy, follow);
            updateGhostMovement(posxf2, posyf2, up2, down2, left2, right2, posx, posy, follow);
            updateGhostMovement(posxf3, posyf3, up3, down3, left3, right3, posx, posy, follow);
            updateGhostMovement(posxf4, posyf4, up4, down4, left4, right4, posx, posy, follow);
        }



        // desenha os Fantasmas
        jacare.setPosition(posyf * SIZE, posxf * SIZE);
        window.draw(jacare);

        jacare2.setPosition(posyf2 * SIZE, posxf2 * SIZE);
        window.draw(jacare2);

        jacare3.setPosition(posyf3 * SIZE, posxf3 * SIZE);
        window.draw(jacare3);

        jacare4.setPosition(posyf4 * SIZE, posxf4 * SIZE);
        window.draw(jacare4);

        if (!gameOver && !win) {
            if (checkCollision(posyf, posxf, posy, posx) ||
                checkCollision(posyf2, posxf2, posy, posx) ||
                checkCollision(posyf3, posxf3, posy, posx) ||
                checkCollision(posyf4, posxf4, posy, posx)) {
                cont++;
                gameOver = true;
            }
        }

        if (gameOver) {
            if (musicSiren.getStatus() == sf::SoundSource::Playing) {
                musicSiren.stop();
            }
            if (musicDie.getStatus() != sf::SoundSource::Playing && count == 0) {
                musicDie.play();
                count++;
            }
            else if (musicDie.getStatus() == sf::SoundSource::Playing) {
                sf::sleep(sf::seconds(2));
                musicDie.stop();
            }

            sf::Texture capySad;
            if (!capySad.loadFromFile("images/diePic.png")) {
                std::cout << "Erro lendo imagem diePic.png\n";
                return 0;
            }
            sf::Sprite diePic;
            diePic.setTexture(capySad);
            diePic.setPosition( 5 * SIZE, 8 * SIZE);
            window.draw(diePic);

            sf::Text loose;
            loose.setFont(font);
            loose.setCharacterSize(100);
            loose.setFillColor(sf::Color::Red);
            loose.setString("YOU LOSE!");
            loose.setPosition(200, 200);
            window.draw(loose);
        }


        // pacman come as pilulas
        if (mapa[posx][posy] == '0') {
            score += 10;
            if (score % 20 == 0) {
                musicMove.play();
            }
            mapa[posx][posy] = vazio;

        }



        // Desenha o contador de pílulas coletadas
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setPosition(20, 5);
        window.draw(scoreText);

        if (score == 1650) {
            cont++;
            musicSiren.stop();

            sf::Texture capyHappy;
            if (!capyHappy.loadFromFile("images/winPic.png")) {
                std::cout << "Erro lendo imagem winPic.png\n";
                return 0;
            }
            sf::Sprite winPic;
            winPic.setTexture(capyHappy);
            winPic.setPosition(5 * SIZE, 8 * SIZE);
            window.draw(winPic);

            win = true;
            sf::Text win;
            scoreText.setFont(font);
            scoreText.setCharacterSize(100);
            scoreText.setFillColor(sf::Color::Green);
            scoreText.setString("YOU WIN!!!");
            scoreText.setPosition(240, 200);
            window.draw(scoreText);
        }

        if (!start) {
            sf::Text ready;
            ready.setFont(font);
            ready.setCharacterSize(25);
            ready.setFillColor(sf::Color::Yellow);
            ready.setString("READY???");
            ready.setPosition(410, 410);
            window.draw(ready);
        }



        if (restart) {
            // Reseta as variaiveis
            posx = 17;
            posy = 13;
            cima = false;
            baixo = false;
            esq = false;
            dir = false;
            up = true;
            down = false;
            left = false;
            right = false;
            up2 = true;
            down2 = false;
            left2 = false;
            right2 = false;
            up3 = true;
            down3 = false;
            left3 = false;
            right3 = false;
            up4 = true;
            down4 = false;
            left4 = false;
            right4 = false;
            count = 0;
            cont = 0;
            follow = false;
            score = 0;
            gameOver = false;
            win = false;
            start = false;

            // Reseta as posições dos fantasmas
            posxf = 7;
            posyf = 9;
            posxf2 = 9;
            posyf2 = 9;
            posxf3 = 9;
            posyf3 = 10;
            posxf4 = 9;
            posyf4 = 8;

            // Reseta o mapa
            for (int i = 0; i < 19; i++) {
                for (int j = 0; j < 19; j++) {
                    if (mapa[i][j] == '2') {
                        mapa[i][j] = '0';
                    }
                }
            }

            // Reseta as musicas
            music.stop();
            musicDie.stop();
            musicMove.stop();
            musicSiren.stop();

            // Reseta os clocks
            clock.restart();
            timer.restart();
            followTimer.restart();

            restart = false;
        }

        // termina e desenha o frame corrente
        window.display();

    }

    return 0;
}

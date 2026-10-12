#include <iostream>



struct Mob {
    int x;
    int y;
    int health;
    char name;
};

struct Tile {
    char name;
};

const Tile water = { '~' };
const Tile mountain = { 'M' };
const Tile ground = { '.' };


void afficherMap(int map[10][20], Mob player) {
    for (int ligne = 0; ligne < 10; ++ligne) {
        for (int colonne = 0; colonne < 20; ++colonne) {
            if (ligne == player.x and colonne == player.y) {
                std::cout << player.name << ' ';
            }
            else if (map[ligne][colonne] == 1) {
                std::cout << water.name << ' ';
            }
            else if (map[ligne][colonne] == 2) {
                std::cout << mountain.name << ' ';
            }
            else {
                std::cout << ground.name << ' ';
            }
            
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << player.x << ' ';
    std::cout << player.y << ' ';
}


void updatePlayer(Mob& player, char order) {
    if (order == 'd') {
        player.y += 1;
    }
    else if (order == 'q') {
        player.y -= 1;
    }
    else if (order == 'z') {
        player.x -= 1;
    }
    else {
        player.x += 1;
    }
}


int main()
{
    int map[10][20] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    Mob player = { 2, 4, 1, 'H'};

    char order;

    while (player.health != 0) {

        system("cls");

        afficherMap(map, player);

        std::cin >> order;

        updatePlayer(player, order);


    }
    return 1;
}
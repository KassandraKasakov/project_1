#include <iostream>



struct Mob {
    int x;
    int y;
    int health;
    char name;
};

struct Tile {
    int value;
    char name;
    bool walkable = true;
};

const Tile ground = { 0, '.' };
const Tile water = { 1, '~' };
const Tile mountain = { 2, 'M', false };

Tile const tiles[3] = { ground, water, mountain };


void afficherMap(int map[10][20], Mob player) {
    for (int ligne = 0; ligne < 10; ++ligne) {
        for (int colonne = 0; colonne < 20; ++colonne) {
            if (ligne == player.x and colonne == player.y) {
                std::cout << player.name << ' ';
            }
            else {
                for (int t = 0; t < 3; t++) {
                    if (map[ligne][colonne] == tiles[t].value)
                        std::cout << tiles[t].name << ' ';
                }
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Coords : " << player.x << ' ' << player.y << std::endl;
    std::cout << "Health : " << player.health << "/1" << std::endl;
    std::cout << "Next Order : ";
}


void updatePlayer(Mob& player, char order) {
    if (order == 'd' and player.y + 1 <= 19) {
        player.y += 1;
    }
    else if (order == 'q' and player.y - 1 >= 0) {
        player.y -= 1;
    }
    else if (order == 'z' and player.x - 1 >= 0) {
        player.x -= 1;
    }
    else if (order == 's' and player.x + 1 <= 9) {
        player.x += 1;
    }
    else if (order == 'r') {
        player.health = 0;
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
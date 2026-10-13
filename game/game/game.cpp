#include <iostream>



struct Entity {
    int x;
    int y;
    int health;
    char name;
};

struct Mob {
    Entity enemy;
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


void afficherMap(int map[10][20], Entity player, Mob enemies[5]) {
    for (int ligne = 0; ligne < 10; ++ligne) {
        for (int colonne = 0; colonne < 20; ++colonne) {

            // Detection enemis
            bool enemy = false;

            for (int i = 0; i < 5; i++) {
                if (ligne == enemies[i].enemy.x and colonne == enemies[i].enemy.y) {
                    enemy = true;
                }
            }

            // Affichage enemis
            if (enemy) {
                std::cout << 'X' << ' ';
            }

            // Affichage joueur
            else if (ligne == player.x and colonne == player.y) {
                std::cout << player.name << ' ';
            }

            // Affichage Map
            else {
                for (int t = 0; t < 3; t++) {
                    if (map[ligne][colonne] == tiles[t].value)
                        std::cout << tiles[t].name << ' ';
                }
            }
        }
        std::cout << std::endl;
    }

    // Affichage externe
    std::cout << std::endl;
    std::cout << "Coords : " << player.x << ' ' << player.y << std::endl;
    std::cout << "Health : " << player.health << "/1" << std::endl;
    std::cout << "Next Order : ";
}


void updatePlayer(Entity& player, char order, int map[10][20]) {

    // Déplacements (droite / gauche / haut / bas)
    if (order == 'd' and player.y + 1 <= 19) {
        for (int t = 0; t < 3; t++) {
            if (map[player.x][player.y + 1] == tiles[t].value and tiles[t].walkable) {
                player.y += 1;
                break;
            }
        }
    }
    else if (order == 'q' and player.y - 1 >= 0) {
        for (int t = 0; t < 3; t++) {
            if (map[player.x][player.y - 1] == tiles[t].value and tiles[t].walkable) {
                player.y -= 1;
                break;
            }
        }
    }
    else if (order == 'z' and player.x - 1 >= 0) {
        for (int t = 0; t < 3; t++) {
            if (map[player.x - 1][player.y] == tiles[t].value and tiles[t].walkable) {
                player.x -= 1;
                break;
            }
        }
    }
    else if (order == 's' and player.x + 1 <= 9) {
        for (int t = 0; t < 3; t++) {
            if (map[player.x + 1][player.y] == tiles[t].value and tiles[t].walkable) {
                player.x += 1;
                break;
            }
        }
    }

    // Reset (tue le joueur)
    else if (order == 'r') {
        player.health = 0;
    }
}


void collision(Entity& player, Mob enemies[5]) {
    for (int i = 0; i < 5; i++) {
        if (player.x == enemies[i].enemy.x and player.y == enemies[i].enemy.y) {
            player.health = 0;
        }
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

    Entity player = { 2, 4, 1, 'H'};

    Mob enemies[5] = {
        {2, 10, 1, 'X'},
        {9, 1, 1, 'X'},
        {0, 0, 1, 'X'},
        {4, 15, 1, 'X'},
        {9, 19, 1, 'X'},
    };

    char order;


    afficherMap(map, player, enemies);

    while (player.health != 0) {

        std::cin >> order;

        updatePlayer(player, order, map);
        collision(player, enemies);

        system("cls");

        afficherMap(map, player, enemies);




    }
    return 0;
}
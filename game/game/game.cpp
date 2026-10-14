#include <iostream>
#include <cstdlib>
#include <ctime>
#include <curses.h>

struct Map {
    int* data;
    int width;
    int height;
};

struct Entity {
    int x;
    int y;
    int health;
    char name;
};

struct Mob {
    Entity enemy;
    int damage;
};

enum Tile_Type
{
    Ground = 0,
    Water = 1,
    Mountain = 2,
    Door = 3
};
struct Tile {
    char name;
    bool walkable = true;
    bool victory = false;
};

const Tile ground = {'.'};
const Tile water = {'~'};
const Tile mountain = {'M', false};
const Tile door = {'E', true, true};

Tile const tiles[4] = {ground, water, mountain, door };



int getCell(int x, int y, int columnMax)
{
    return y * columnMax + x;
}


void afficherMap(int map[], Entity player, Mob enemies[5]) {

    for (int ligne = 0; ligne < 10; ++ligne) {
        for (int colonne = 0; colonne < 20; ++colonne) {

            int index = getCell(colonne, ligne, 20);

            // Detection enemis
            bool isenemy = false;
            int whenenemy;

            for (int i = 0; i < 5; i++) {
                if (ligne == enemies[i].enemy.x and colonne == enemies[i].enemy.y) {
                    isenemy = true;
                    whenenemy = i;
                }
            }

            // Affichage enemis
            if (isenemy) {
                std::cout << enemies[whenenemy].enemy.name << ' ';
            }

            // Affichage joueur
            else if (ligne == player.x and colonne == player.y) {
                std::cout << player.name << ' ';
            }

            // Affichage Map
            else {
                for (int t = 0; t < 4; t++) {
                    if (map[index] == tiles[t].value)
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


void updatePlayer(Entity& player, char order, int map[], bool &victory) {

    int xDir = 0;
    int yDir = 0;
    switch (order)
    {
        case 'd':
            xDir = 1;
            break;
        case 'q':
            xDir = -1;
            break;
        case 'z':
            yDir = 1;
            break;
        case 's':
            yDir = -1;
            break;
    }
    int newX = player.x + xDir;
    int newY = player.y + yDir;

    int tileType = map[getCell(newX, newY, 20)];
    if (tiles[tileType].walkable)
    {
        player.x = newX;
        player.y = newY;
    }
    

    // Victory Tile ?
        if (map[getCell(player.x, player.y, 20)] == -1) {
            victory = true;
        }

    // Reset (tue le joueur)
    if (order == 'r') {
        player.health = 0;
    }
}


bool isOtherEnemy(Mob enemies[5], int time, int current_x, int current_y) {
    for (int i = 0; i < 5; i++) {
        if (i != time) {
            if (enemies[i].enemy.x == current_x and enemies[i].enemy.y == current_y) {
                return false;
            }
        }
    }
    return true;
}


void updateEnemies(Mob enemies[5], int map[]) {
    int random;

    for (int i = 0; i < 5; i++) {


        random = rand() % 4;

        // Déplacements aléatoires (0 Haut / 1 Bas / 2 Droite / 3 Gauche)
        if (random == 0 and enemies[i].enemy.x - 1 >= 0) {
            for (int t = 0; t < 4; t++) {
                if (map[getCell(enemies[t].enemy.x - 1, enemies[t].enemy.y, 20)] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x - 1, enemies[i].enemy.y)) {
                        enemies[i].enemy.x -= 1;
                        break;
                    }
                }
            }
        }
        else if (random == 1 and enemies[i].enemy.x + 1 <= 9) {
            for (int t = 0; t < 4; t++) {
                if (map[getCell(enemies[t].enemy.x + 1, enemies[t].enemy.y, 20)] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x + 1, enemies[i].enemy.y)) {
                        enemies[i].enemy.x += 1;
                        break;
                    }
                }
            }
        }
        else if (random == 2 and enemies[i].enemy.y + 1 <= 19) {
            for (int t = 0; t < 4; t++) {
                if (map[getCell(enemies[t].enemy.x, enemies[t].enemy.y + 1, 20)] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x, enemies[i].enemy.y + 1)) {
                        enemies[i].enemy.y += 1;
                        break;
                    }
                }
            }
        }
        else if (random == 3 and enemies[i].enemy.y - 1 >= 0) {
            for (int t = 0; t < 4; t++) {
                if (map[getCell(enemies[t].enemy.x, enemies[t].enemy.y - 1, 20)] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x, enemies[i].enemy.y - 1)) {
                        enemies[i].enemy.y -= 1;
                        break;
                    }
                }
            }
        }
    }
}


void collision(Entity& player, Mob enemies[5]) {
    for (int i = 0; i < 5; i++) {
        if (player.x == enemies[i].enemy.x and player.y == enemies[i].enemy.y) {
            player.health -= enemies[i].damage;
        }
    }
}


void gameOver() {
    system("cls");
    std::cout << '\n' << " Game Over\n Vous avez péri !" << std::endl;
}


int main()
{

    srand(time(0));

    int rowMax = 10;
    int columnMax = 20;

    int map[200] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,2,2,2,
        0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,3,2,
        0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2,
        0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2,
        0,0,0,0,0,0,0,0,0,2,2,1,1,1,1,0,0,0,0,2,
        1,1,1,0,0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,2,
        0,0,1,1,1,1,0,0,0,2,2,1,1,1,1,1,1,1,1,2,
        0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,2,
        0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,2,
        0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,2,2,2,2,2
    };

    int map1[10][20] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,2,2,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,-1,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,2,2,1,1,1,1,0,0,0,0,2},
        {1,1,1,0,0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,2},
        {0,0,1,1,1,1,0,0,0,2,2,1,1,1,1,1,1,1,1,2},
        {0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,2},
        {0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,2,2,2,2,2}
    };

    int currentMap = 0;

    Entity player = { 2, 4, 1, 'H'};

    Mob enemies[5] = {
        {2, 10, 1, 'X', 1},
        {7, 1, 1, 'X', 1},
        {1, 1, 1, 'X', 1},
        {4, 17, 1, 'X', 1},
        {8, 18, 1, 'X', 1},
    };

    char order;
    bool victory = false;


    // Boucle de jeu

    afficherMap(map, player, enemies);

    while (player.health != 0) {

        std::cin >> order;

        updateEnemies(enemies, map);
        updatePlayer(player, order, map, victory);
        collision(player, enemies);

        system("cls");

        afficherMap(map, player, enemies);

        if (victory) {
            victory = false;
            currentMap += 1;
            return 1;
        }

    }
    victory = false;

    if (player.health == 0) {
        gameOver();
        return 0;
    }
    return 1;
}

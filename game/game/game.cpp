#include <iostream>
#include <cstdlib>
#include <ctime>



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


void updateEnemies(Mob enemies[5], int map[10][20]) {
    int random;

    for (int i = 0; i < 5; i++) {


        random = rand() % 4;

        // Déplacements aléatoires (0 Haut / 1 Bas / 2 Droite / 3 Gauche)
        if (random == 0 and enemies[i].enemy.x - 1 >= 0) {
            for (int t = 0; t < 3; t++) {
                if (map[enemies[i].enemy.x - 1][enemies[i].enemy.y] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x - 1, enemies[i].enemy.y)) {
                        enemies[i].enemy.x -= 1;
                        break;
                    }
                }
            }
        }
        else if (random == 1 and enemies[i].enemy.x + 1 <= 9) {
            for (int t = 0; t < 3; t++) {
                if (map[enemies[i].enemy.x + 1][enemies[i].enemy.y] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x + 1, enemies[i].enemy.y)) {
                        enemies[i].enemy.x += 1;
                        break;
                    }
                }
            }
        }
        else if (random == 2 and enemies[i].enemy.y + 1 <= 19) {
            for (int t = 0; t < 3; t++) {
                if (map[enemies[i].enemy.x][enemies[i].enemy.y + 1] == tiles[t].value and tiles[t].walkable) {
                    if (isOtherEnemy(enemies, t, enemies[i].enemy.x, enemies[i].enemy.y + 1)) {
                        enemies[i].enemy.y += 1;
                        break;
                    }
                }
            }
        }
        else if (random == 3 and enemies[i].enemy.y - 1 >= 0) {
            for (int t = 0; t < 3; t++) {
                if (map[enemies[i].enemy.x][enemies[i].enemy.y - 1] == tiles[t].value and tiles[t].walkable) {
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
        {2, 10, 1, 'X', 1},
        {7, 1, 1, 'X', 1},
        {1, 1, 1, 'X', 1},
        {4, 17, 1, 'X', 1},
        {8, 18, 1, 'X', 1},
    };

    char order;


    afficherMap(map, player, enemies);

    // Boucle de jeu
    while (player.health != 0) {

        std::cin >> order;

        updateEnemies(enemies, map);
        updatePlayer(player, order, map);
        collision(player, enemies);

        system("cls");

        afficherMap(map, player, enemies);

    }
    gameOver();
    return 0;
}
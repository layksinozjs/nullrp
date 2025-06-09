#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void clearScreen() {
    system(CLEAR);
}

typedef struct {
    char name[50];
    int power;
    int durability;
    bool ismagic;
} Item;

typedef struct {
    Item *items;
    int capacity;
    int count;
    int equippedIndex; // -1 none
} Inventory;

typedef struct {
    char name[50];
    int health;
    int mana;
    int level;
    Inventory *inventory;
} Character;

typedef struct {
    char name[50];
    int health;
    int attack;
} Enemy;

typedef struct {
    char name[50];
} Map;

Inventory* createInventory(int capacity) {
    Inventory *inv = malloc(sizeof(Inventory));
    inv->items = calloc(capacity, sizeof(Item));
    inv->capacity = capacity;
    inv->count = 0;
    inv->equippedIndex = -1;
    return inv;
}

void addItem(Inventory *inv, Item item) {
    if (inv->count >= inv->capacity) {
        inv->capacity *= 2;
        inv->items = realloc(inv->items, sizeof(Item)*inv->capacity);
    }
    inv->items[inv->count++] = item;
    printf("You obtained: %s\n", item.name);
}

void listInventory(Inventory *inv) {
    if (inv->count == 0) {
        printf("Inventory is empty.\n");
        return;
    }
    printf("Inventory:\n");
    for (int i = 0; i < inv->count; i++) {
        printf("%d) %s (Power: %d, Durability: %d, Magic: %s)%s\n",
               i+1,
               inv->items[i].name,
               inv->items[i].power,
               inv->items[i].durability,
               inv->items[i].ismagic ? "Yes" : "No",
               (i == inv->equippedIndex) ? " [Equipped]" : "");
    }
}

int getEquippedPower(Character *p) {
    if (p->inventory->equippedIndex == -1) return 5;
    return p->inventory->items[p->inventory->equippedIndex].power;
}

void equipItem(Character *p) {
    if (p->inventory->count == 0) {
        printf("No equipment in inventory.\n");
        return;
    }
    listInventory(p->inventory);
    printf("Enter the number of the item you want to equip (0 to cancel): ");
    int c;
    if (scanf("%d", &c) != 1 || c < 0 || c > p->inventory->count) {
        while(getchar() != '\n');
        printf("Invalid choice.\n");
        return;
    }
    while(getchar() != '\n');
    if (c == 0) {
        printf("Equip cancelled.\n");
        return;
    }
    p->inventory->equippedIndex = c - 1;
    printf("%s is now equipped.\n", p->inventory->items[p->inventory->equippedIndex].name);
}

void unequipItem(Character *p) {
    if (p->inventory->equippedIndex == -1) {
        printf("No item is currently equipped.\n");
        return;
    }
    printf("%s was unequipped.\n", p->inventory->items[p->inventory->equippedIndex].name);
    p->inventory->equippedIndex = -1;
}

void fight(Character *p, Enemy enemies[], int enemyCount, int *gold) {
    clearScreen();
    Enemy enemy = enemies[rand() % enemyCount];
    int enemyHealth = enemy.health;
    printf("You encountered: %s!\nEnemy Health: %d, Attack: %d\n", enemy.name, enemyHealth, enemy.attack);

    while (p->health > 0 && enemyHealth > 0) {
        printf("\nPress 'a' to attack, 'r' to run: ");
        char act = getchar();
        while(getchar() != '\n');

        if (act == 'a' || act == 'A') {
            int dmg = (rand() % getEquippedPower(p)) + 1;
            enemyHealth -= dmg;
            printf("You dealt %d damage to the enemy.\n", dmg);

            if (enemyHealth <= 0) {
                printf("You defeated the enemy!\n");
                int earned = rand() % 41; // 0-40 gold
                *gold += earned;
                printf("Gold earned: %d | Total Gold: %d\n", earned, *gold);
                break;
            }

            int edmg = (rand() % enemy.attack) + 1;
            p->health -= edmg;
            printf("%s dealt %d damage to you.\n", enemy.name, edmg);

            if (p->health <= 0) {
                printf("You have been defeated...\n");
                int lost = (*gold >= 100) ? 100 : *gold;
                *gold -= lost;
                printf("Gold lost: %d | Remaining Gold: %d\n", lost, *gold);
                break;
            }
            printf("Your Health: %d | Enemy Health: %d\n", p->health, enemyHealth);
        } else if (act == 'r' || act == 'R') {
            printf("You ran away.\n");
            break;
        } else {
            printf("Invalid key!\n");
        }
    }
    printf("Press Enter to continue...");
    getchar();
    clearScreen();
}

void market(Character *p, Item marketItems[], int count, int *gold) {
    while (1) {
        clearScreen();
        printf("MARKET | Gold: %d\n", *gold);
        for (int i = 0; i < count; i++) {
            int cost = marketItems[i].power * 2;
            printf("%d) %s (Power: %d, Durability: %d) Price: %d gold\n",
                   i+1, marketItems[i].name, marketItems[i].power, marketItems[i].durability, cost);
        }
        printf("0) Exit Market\nYour choice: ");
        int c;
        if (scanf("%d", &c) != 1) {
            while(getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while(getchar() != '\n');

        if (c == 0) break;
        if (c < 1 || c > count) {
            printf("Invalid choice.\n");
            continue;
        }
        Item selected = marketItems[c-1];
        int cost = selected.power * 2;
        if (*gold >= cost) {
            *gold -= cost;
            addItem(p->inventory, selected);
            printf("%s purchased!\n", selected.name);
        } else {
            printf("Not enough gold!\n");
        }
        printf("Press Enter to continue...");
        getchar();
    }
}

void wizard(Character *p, int *gold) {
    clearScreen();
    printf("Wizard Tower\nMana refill costs 10 gold.\nCurrent Mana: %d\nDo you want to refill? (y/n): ", p->mana);
    char ch = getchar();
    while(getchar() != '\n');
    if (ch == 'y' || ch == 'Y') {
        if (*gold >= 10) {
            *gold -= 10;
            p->mana = 100;
            printf("Mana refilled!\n");
        } else {
            printf("Not enough gold.\n");
        }
    } else {
        printf("Cancelled.\n");
    }
    printf("Press Enter to continue...");
    getchar();
}

void loot(Character *p, Item lootItems[], int count, time_t *lastLoot) {
    time_t now = time(NULL);
    double diff = difftime(now, *lastLoot);
    if (diff < 600) {
        printf("You need to wait %d more seconds before looting again.\n", 600 - (int)diff);
        return;
    }
    int i = rand() % count;
    addItem(p->inventory, lootItems[i]);
    *lastLoot = now;
}

int main() {
    srand(time(NULL));

    Item freeItems[4] = {
        {"Fire Blade 🔥", 25, 35, false},
        {"Lominal Axe 🪓", 35, 25, false},
        {"Bow of Magic 🏹", 18, 40, true},
        {"Blessed Katana ⚔️", 20, 40, false}
    };

    Character freeCharacters[4] = {
        {"Kevin 😎", 90, 10, 1, NULL},
        {"Londa 🌸", 75, 30, 1, NULL},
        {"Huzk 🧙", 100, 5, 1, NULL},
        {"Garret 💪", 130, 0, 1, NULL}
    };

    Enemy enemies[4] = {
        {"Lorenzo The Great", 75, 10},
        {"Evil Kevin", 55, 25},
        {"Millenium", 65, 15},
        {"Zopeda", 70, 20}
    };

    Map maps[4] = {
        {"Chilly 🤙"},
        {"Forest 🌴"},
        {"Desert 🏜️"},
        {"Island 🏝️"}
    };

    Item lootItems[4] = {
        {"Rusty Sword", 10, 10, false},
        {"Healing Potion", 0, 0, true},
        {"Magic Staff", 15, 20, true},
        {"Steel Dagger", 12, 15, false}
    };

    Item marketItems[4] = {
        {"Iron Sword", 20, 25, false},
        {"Silver Axe", 30, 20, false},
        {"Enchanted Bow", 25, 30, true},
        {"Golden Katana", 35, 40, false}
    };

    int gold = 0;
    time_t lastLootTime = 0;

    clearScreen();
    printf("\nWelcome to NullRP\n\n");

    // Select Character
    printf("Select your character:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d) %s (HP: %d, Mana: %d, Level: %d)\n", i+1, freeCharacters[i].name, freeCharacters[i].health, freeCharacters[i].mana, freeCharacters[i].level);
    }
    int ch;
    do {
        printf("Your choice: ");
        if (scanf("%d", &ch) != 1 || ch < 1 || ch > 4) {
            while(getchar() != '\n');
            printf("Invalid choice.\n");
        } else break;
    } while(1);
    while(getchar() != '\n');

    Character player = freeCharacters[ch - 1];
    player.inventory = createInventory(10);

    // Select starting item
    printf("\nSelect your starting item:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d) %s (Power: %d, Durability: %d, Magic: %s)\n",
               i+1, freeItems[i].name, freeItems[i].power, freeItems[i].durability, freeItems[i].ismagic ? "Yes" : "No");
    }
    int itemCh;
    do {
        printf("Your choice: ");
        if (scanf("%d", &itemCh) != 1 || itemCh < 1 || itemCh > 4) {
            while(getchar() != '\n');
            printf("Invalid choice.\n");
        } else break;
    } while(1);
    while(getchar() != '\n');

    addItem(player.inventory, freeItems[itemCh - 1]);
    player.inventory->equippedIndex = 0;

    // Select map
    printf("\nSelect your map:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d) %s\n", i+1, maps[i].name);
    }
    int mapCh;
    do {
        printf("Your choice: ");
        if (scanf("%d", &mapCh) != 1 || mapCh < 1 || mapCh > 4) {
            while(getchar() != '\n');
            printf("Invalid choice.\n");
        } else break;
    } while(1);
    while(getchar() != '\n');

    Map selectedMap = maps[mapCh - 1];

    clearScreen();

    int running = 1;
    while(running) {
        printf("Main Menu - Map: %s | Health: %d | Mana: %d | Gold: %d\n\n", selectedMap.name, player.health, player.mana, gold);
        printf("1) Fight\n");
        printf("2) Inventory\n");
        printf("3) Equip Item\n");
        printf("4) Unequip Item\n");
        printf("5) Market\n");
        printf("6) Wizard\n");
        printf("7) Loot (Once every 10 minutes)\n");
        printf("8) Save Game\n");
        printf("0) Exit\n");
        printf("\nChoose an option: ");
        int opt;
        if (scanf("%d", &opt) != 1) {
            while(getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while(getchar() != '\n');

        switch(opt) {
            case 1:
                fight(&player, enemies, 4, &gold);
                break;
            case 2:
                clearScreen();
                listInventory(player.inventory);
                printf("\nPress Enter to continue...");
                getchar();
                clearScreen();
                break;
            case 3:
                clearScreen();
                equipItem(&player);
                printf("\nPress Enter to continue...");
                getchar();
                clearScreen();
                break;
            case 4:
                clearScreen();
                unequipItem(&player);
                printf("\nPress Enter to continue...");
                getchar();
                clearScreen();
                break;
            case 5:
                market(&player, marketItems, 4, &gold);
                clearScreen();
                break;
            case 6:
                wizard(&player, &gold);
                clearScreen();
                break;
            case 7:
                clearScreen();
                loot(&player, lootItems, 4, &lastLootTime);
                printf("\nPress Enter to continue...");
                getchar();
                clearScreen();
                break;
            case 8:
                clearScreen();
                printf("Game saved!\nPress Enter to continue...");
                getchar();
                clearScreen();
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    free(player.inventory->items);
    free(player.inventory);

    printf("Thanks for playing!\n");
    return 0;
}

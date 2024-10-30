/*
    game.c

    Tugas Project Game Konsep Pemrograman

    Anggota Kelompok:
    - Muhammad Daffa Rahman (L0124062)
    - Phyrurizqi Altiano Firdauzan (L0124069)
    - Resyad Agung Wicaksono (L0124072)
    - Raditya Adi Pradana (L0124070)
    - Daffa Arkan Taqiya (L0124047)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define STRING_SHORT 32
#define STRING_MAX 255

#define DEFAULT_BAR 219
#define MENU_BAR '='

#define CURRENCY "$"
#define DEFAULT_MONEY 1000

#define MEDKIT_PRICE 100
#define MEDKIT_VALUE 10

// colors
#define COLOR_BLACK 30
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_YELLOW 33
#define COLOR_BLUE 34
#define COLOR_PURPLE 35
#define COLOR_CYAN 36
#define COLOR_WHITE 37

#define COLOR_RESET 0
#define COLOR_TITLE COLOR_YELLOW

// game
#define ULTIMATE_MULTIPLIER 3

struct Character
{
    char *name;
    int health;
    int attack;
    int defense;
    int price;
};

struct Item
{
    char *name;
    int value;
    int price;
};

int GetCharacters(char *path, struct Character (*characters)[])
{
    FILE *fptr;
    fptr = fopen(path, "r");
    if (!fptr)
        return 0;

    char buffer[STRING_MAX];
    int counter = 0;
    while (fgets(buffer, STRING_MAX, fptr))
    {
        char *buffer_name = (char *)malloc(sizeof(char) * STRING_MAX);
        int buffer_health, buffer_attack, buffer_defense, buffer_price;

        buffer[strcspn(buffer, "\n")] = 0;
        sscanf(buffer, "%[^,],%d,%d,%d,%d", buffer_name, &buffer_health, &buffer_attack, &buffer_defense, &buffer_price);

        (*characters)[counter].name = buffer_name;
        (*characters)[counter].health = buffer_health;
        (*characters)[counter].attack = buffer_attack;
        (*characters)[counter].defense = buffer_defense;
        (*characters)[counter].price = buffer_price;

        counter++;
    }

    fclose(fptr);
    return counter;
}

int GetItems(char *path, struct Item (*items)[])
{
    FILE *fptr;
    fptr = fopen(path, "r");
    if (!fptr)
        return 0;

    char buffer[STRING_MAX];
    int counter = 0;
    while (fgets(buffer, STRING_MAX, fptr))
    {
        char *buffer_name = (char *)malloc(sizeof(char) * STRING_MAX);
        int buffer_value, buffer_price;

        buffer[strcspn(buffer, "\n")] = 0;
        sscanf(buffer, "%[^,],%d,%d", buffer_name, &buffer_value, &buffer_price);

        (*items)[counter].name = buffer_name;
        (*items)[counter].value = buffer_value;
        (*items)[counter].price = buffer_price;

        counter++;
    }

    fclose(fptr);
    return counter;
}

void ReadPrintFile(char *path)
{
    FILE *fptr;
    fptr = fopen(path, "r");
    if (!fptr)
        return;

    char buffer[STRING_MAX];
    while (fgets(buffer, STRING_MAX, fptr))
    {
        printf("%s", buffer);
    }

    fclose(fptr);
}

void ClearScreen()
{
#ifdef _WIN32
    system("cls");
#endif

#ifndef _WIN32
    system("clear");
#endif
}

void SetColor(int code)
{
    printf("\033[0;%dm", code);
}

void ResetColor()
{
    SetColor(COLOR_RESET);
}

void SetHealthColor(int health, int healthMax)
{
    if (health <= healthMax && health >= healthMax * 0.6f)
        SetColor(COLOR_GREEN);
    else if (health < healthMax * 0.6f && health >= healthMax * 0.3f)
        SetColor(COLOR_YELLOW);
    else
        SetColor(COLOR_RED);
}

void DrawBar(char bar, int n)
{
    for (int i = 0; i < n; i++)
        printf("%c", bar);
}

void PromptEnter(char *message, int afterScanf)
{
    printf("%s", message);
    char enter = 0;
    while (enter != '\r' && enter != '\n')
    {
        // to prevent trailing '\n' from scanf is captured early
        if (afterScanf == 1)
            enter = getchar();
        enter = getchar();
    }
}

void PromptSuccessEnter(char *message, int afterScanf)
{
    SetColor(COLOR_GREEN);
    PromptEnter(message, afterScanf);
    ResetColor();
}

void PromptErrorEnter(char *message, int afterScanf)
{
    SetColor(COLOR_RED);
    PromptEnter(message, afterScanf);
    ResetColor();
}

int GetUltimateAttack(int defaultAttack)
{
    int ulitmateAttack = (rand() % (defaultAttack * ULTIMATE_MULTIPLIER)) + 1;
    return ulitmateAttack;
}

int main()
{
    // preparing characters, weapons, defenses
    struct Character characters[64];
    struct Item weapons[64], defenses[64];
    int loadedCharacters = GetCharacters("GameData/Characters.txt", &characters);
    int loadedWeapons = GetItems("GameData/Weapons.txt", &weapons);
    int loadedDefenses = GetItems("GameData/Defenses.txt", &defenses);

GameStart:
    // player1
    int P1Money = DEFAULT_MONEY;

    int P1SelectedCharacter = 0;
    int P1SelectedWeapon = 0;
    int P1SelectedDefense = 0;

    int P1Health = 0;
    int P1Attack = 0;
    int P1Defense = 0;

    // player 2
    int P2Money = DEFAULT_MONEY;

    int P2SelectedCharacter = 0;
    int P2SelectedWeapon = 0;
    int P2SelectedDefense = 0;

    int P2Health = 0;
    int P2Attack = 0;
    int P2Defense = 0;

// MAIN MENU
MenuMain:
    ClearScreen();
    SetColor(COLOR_TITLE);
    // game title
    ReadPrintFile("GameEtc/Logo.txt");
    printf("\n\n");
    ResetColor();
    // menu
    printf("(1) -> Play Game\n");
    printf("(2) -> Credits\n");
    printf("(3) -> Quit\n\n");
    // selection
    char *menuInput = (char *)malloc(sizeof(char) * STRING_SHORT);
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) > 0 && atoi(menuInput) <= 3))
        goto MenuMain;

    if (atoi(menuInput) == 1)
        goto MenuCharacter1;

    if (atoi(menuInput) == 2)
    {
        ClearScreen();
        ReadPrintFile("GameEtc/Credits.txt");
        printf("\n\n");
        PromptEnter("Press Enter to go back to main menu . . .", 1);
        goto MenuMain;
    }

    if (atoi(menuInput) == 3)
        return 0;

MenuCharacter1:
    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Player 1.... Choose Your Character!\n");
    DrawBar(MENU_BAR, 37);
    ResetColor();
    printf("\n\n");

    SetColor(COLOR_GREEN);
    printf("Player 1 Money: $%d\n\n", P1Money);
    ResetColor();

    for (int i = 0; i < loadedCharacters; i++)
    {
        printf("(%d) -> %s ($%d)\n\tHp: %3d | Attack: %3d | Defense: %3d\n", i + 1, characters[i].name, characters[i].price, characters[i].health, characters[i].attack, characters[i].defense);
    }

    printf("\n(%d) -> Back to previous menu\n\n", loadedCharacters + 1);

    // input
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) >= 1 && atoi(menuInput) <= loadedCharacters + 1))
        goto MenuCharacter1;

    if (atoi(menuInput) == loadedCharacters + 1)
        goto MenuMain;

    // check if money is enough to buy
    P1SelectedCharacter = atoi(menuInput) - 1;
    if (P1Money >= characters[P1SelectedCharacter].price)
    {
        P1Money -= characters[P1SelectedCharacter].price;
        goto MenuCharacter2;
    }
    else
    {
        PromptErrorEnter("\nNot Enough Money! . . . (Press Enter)", 1);
        goto MenuCharacter1;
    }

MenuCharacter2:
    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Player 2.... Choose Your Character!\n");
    DrawBar(MENU_BAR, 37);
    ResetColor();
    printf("\n\n");

    SetColor(COLOR_GREEN);
    printf("Player 2 Money: $%d\n\n", P2Money);
    ResetColor();

    for (int i = 0; i < loadedCharacters; i++)
    {
        if (i != P1SelectedCharacter)
            printf("(%d) -> %s ($%d)\n\tHp: %3d | Attack: %3d | Defense: %3d\n", i + 1, characters[i].name, characters[i].price, characters[i].health, characters[i].attack, characters[i].defense);
        else
            printf("(%d) -> Already Selected by Player 1!!!\n\n", i + 1);
    }

    printf("\n(%d) -> Back to previous menu\n\n", loadedCharacters + 1);

    // input
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) >= 1 && atoi(menuInput) <= loadedCharacters + 1))
        goto MenuCharacter2;

    if (atoi(menuInput) == loadedCharacters + 1)
    {
        P1Money += characters[P1SelectedCharacter].price;
        goto MenuCharacter1;
    }

    if (atoi(menuInput) - 1 == P1SelectedCharacter)
        goto MenuCharacter2;

    // check if money is enough to buy
    P2SelectedCharacter = atoi(menuInput) - 1;
    if (P2Money >= characters[P2SelectedCharacter].price)
    {
        P2Money -= characters[P2SelectedCharacter].price;
        goto MenuWeapon1;
    }
    else
    {
        PromptErrorEnter("\nNot Enough Money! . . . (Press Enter)", 1);
        goto MenuCharacter2;
    }

MenuWeapon1:
    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Player 1.... Choose Your Weapon!\n");
    DrawBar(MENU_BAR, 37);
    ResetColor();
    printf("\n\n");

    SetColor(COLOR_GREEN);
    printf("Player 1 Money: $%d\n\n", P1Money);
    ResetColor();

    for (int i = 0; i < loadedWeapons; i++)
    {
        printf("(%d) -> %s ($%d)\tPower: %3d\n", i + 1, weapons[i].name, weapons[i].price, weapons[i].value);
    }

    printf("\n(%d) -> Back to previous menu\n\n", loadedWeapons + 1);

    // input
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) >= 1 && atoi(menuInput) <= loadedWeapons + 1))
        goto MenuWeapon1;

    if (atoi(menuInput) == loadedWeapons + 1)
    {
        P2Money += characters[P2SelectedCharacter].price;
        goto MenuCharacter2;
    }

    P1SelectedWeapon = atoi(menuInput) - 1;
    if (P1Money >= weapons[P1SelectedWeapon].price)
    {
        P1Money -= weapons[P1SelectedWeapon].price;
        goto MenuWeapon2;
    }
    else
    {
        PromptErrorEnter("\nNot Enough Money! . . . (Press Enter)", 1);
        goto MenuWeapon1;
    }

MenuWeapon2:
    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Player 2.... Choose Your Weapon!\n");
    DrawBar(MENU_BAR, 37);
    ResetColor();
    printf("\n\n");

    SetColor(COLOR_GREEN);
    printf("Player 2 Money: $%d\n\n", P2Money);
    ResetColor();

    for (int i = 0; i < loadedWeapons; i++)
    {
        printf("(%d) -> %s ($%d)\tPower: %3d\n", i + 1, weapons[i].name, weapons[i].price, weapons[i].value);
    }

    printf("\n(%d) -> Back to previous menu\n\n", loadedWeapons + 1);

    // input
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) >= 1 && atoi(menuInput) <= loadedWeapons + 1))
        goto MenuWeapon2;

    if (atoi(menuInput) == loadedWeapons + 1)
    {
        P1Money += weapons[P1SelectedWeapon].price;
        goto MenuWeapon1;
    }

    P2SelectedWeapon = atoi(menuInput) - 1;
    if (P2Money >= weapons[P2SelectedWeapon].price)
    {
        P2Money -= weapons[P2SelectedWeapon].price;
        goto MenuDefense1;
    }
    else
    {
        PromptErrorEnter("\nNot Enough Money! . . . (Press Enter)", 1);
        goto MenuWeapon2;
    }

MenuDefense1:
    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Player 1.... Choose Your Defense!\n");
    DrawBar(MENU_BAR, 37);
    ResetColor();
    printf("\n\n");

    SetColor(COLOR_GREEN);
    printf("Player 1 Money: $%d\n\n", P1Money);
    ResetColor();

    for (int i = 0; i < loadedDefenses; i++)
    {
        printf("(%d) -> %s ($%d)\tPower: %d\n", i + 1, defenses[i].name, defenses[i].price, defenses[i].value);
    }

    printf("\n(%d) -> Back to previous menu\n\n", loadedDefenses + 1);

    // input
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) >= 1 && atoi(menuInput) <= loadedDefenses + 1))
        goto MenuDefense1;

    if (atoi(menuInput) == loadedDefenses + 1)
    {
        P2Money += weapons[P2SelectedWeapon].price;
        goto MenuWeapon2;
    }

    P1SelectedDefense = atoi(menuInput) - 1;
    if (P1Money >= defenses[P1SelectedDefense].price)
    {
        P1Money -= defenses[P1SelectedDefense].price;
        goto MenuDefense2;
    }
    else
    {
        PromptErrorEnter("\nNot Enough Money! . . . (Press Enter)", 1);
        goto MenuDefense1;
    }

MenuDefense2:
    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Player 2.... Choose Your Defense!\n");
    DrawBar(MENU_BAR, 37);
    ResetColor();
    printf("\n\n");

    SetColor(COLOR_GREEN);
    printf("Player 2 Money: $%d\n\n", P2Money);
    ResetColor();

    for (int i = 0; i < loadedDefenses; i++)
    {
        printf("(%d) -> %s ($%d)\tPower: %d\n", i + 1, defenses[i].name, defenses[i].price, defenses[i].value);
    }

    printf("\n(%d) -> Back to previous menu\n\n", loadedDefenses + 1);

    // input
    printf("Your Choice: ");
    scanf("%s", menuInput);

    if (!(atoi(menuInput) >= 1 && atoi(menuInput) <= loadedDefenses + 1))
        goto MenuDefense2;

    if (atoi(menuInput) == loadedDefenses + 1)
    {
        P1Money += defenses[P1SelectedDefense].price;
        goto MenuDefense1;
    }

    P2SelectedDefense = atoi(menuInput) - 1;
    if (P2Money >= defenses[P2SelectedDefense].price)
    {
        P2Money -= defenses[P2SelectedDefense].price;
        goto MenuConfirm;
    }
    else
    {
        PromptErrorEnter("\nNot Enough Money! . . . (Press Enter)", 1);
        goto MenuDefense2;
    }

MenuConfirm:
    // Set Everything Up

    // Player 1
    P1Health = characters[P1SelectedCharacter].health;
    P1Attack = characters[P1SelectedCharacter].attack + weapons[P1SelectedWeapon].value;
    P1Defense = characters[P1SelectedDefense].defense + defenses[P1SelectedDefense].value;

    // Player 2
    P2Health = characters[P2SelectedCharacter].health;
    P2Attack = characters[P2SelectedCharacter].attack + weapons[P2SelectedWeapon].value;
    P2Defense = characters[P2SelectedDefense].defense + defenses[P2SelectedDefense].value;

    P1Attack = abs(P1Attack - P2Defense);
    P2Attack = abs(P2Attack - P2Defense);

    ClearScreen();
    SetColor(COLOR_TITLE);
    printf("Confirmation Screen\n");
    DrawBar(MENU_BAR, 22);
    ResetColor();
    printf("\n\n");

    printf("Player 1\n");
    printf("Money     : $%d\n", P1Money);
    printf("Character : %s\n", characters[P1SelectedCharacter].name);
    printf("Weapon    : %s\n", weapons[P1SelectedWeapon].name);
    printf("Defense   : %s\n", defenses[P1SelectedDefense].name);
    printf("\nTotal Attack : %d\n\n", P1Attack);

    printf("Player 2\n");
    printf("Money     : $%d\n", P2Money);
    printf("Character : %s\n", characters[P2SelectedCharacter].name);
    printf("Weapon    : %s\n", weapons[P2SelectedWeapon].name);
    printf("Defense   : %s\n", defenses[P2SelectedDefense].name);
    printf("\nTotal Attack : %d\n\n", P2Attack);

    printf("confirm (y/n)? -> ");
    scanf("%s", menuInput);
    if (strcmp(menuInput, "n") == 0)
    {
        P2Money += defenses[P2SelectedDefense].price;
        goto MenuDefense2;
    }

    if (strcmp(menuInput, "y") == 0)
        goto GamePlay;

    goto MenuConfirm;

GamePlay:
    srand(time(0));
    int currentPlayer = (rand() % 2) + 1;

    while (P1Health > 0 && P2Health > 0)
    {
        ClearScreen();
        SetColor(COLOR_TITLE);
        printf(
            "    __________________  ________\n"
            "   / ____/  _/ ____/ / / /_  __/\n"
            "  / /_   / // / __/ /_/ / / /   \n"
            " / __/ _/ // /_/ / __  / / /    \n"
            "/_/   /___/\\____/_/ /_/ /_/     \n\n");
        ResetColor();

        printf("(Player 1) %s \n", characters[P1SelectedCharacter].name);
        SetHealthColor(P1Health, characters[P1SelectedCharacter].health);
        DrawBar(DEFAULT_BAR, P1Health);
        printf(" %d", P1Health);
        ResetColor();
        printf("\n\n");

        printf("(Player 2) %s\n", characters[P2SelectedCharacter].name);
        SetHealthColor(P2Health, characters[P2SelectedCharacter].health);
        DrawBar(DEFAULT_BAR, P2Health);
        printf(" %d", P2Health);
        ResetColor();
        printf("\n\n");

        printf("\n");

        printf("It's Player %d Turn....!!!\n", (currentPlayer == 1) ? 1 : 2);

        SetColor(COLOR_GREEN);
        printf("Money : %s%d\n", CURRENCY, (currentPlayer == 1) ? P1Money : P2Money);
        ResetColor();

        printf("\n");
        printf("(1) -> Basic Attack (%d)\n", (currentPlayer == 1) ? P1Attack : P2Attack);
        printf("(2) -> Ultimate Attack (from 1 up to %d & penalty -%d Hp)\n", ((currentPlayer == 1) ? P1Attack : P2Attack) * 3, (currentPlayer == 1) ? P1Attack : P2Attack);
        printf("(3) -> Heal +%d (buy medkit $%d)\n", MEDKIT_VALUE, MEDKIT_PRICE);
        printf("(4) -> Flee (Skip)\n");

        printf("\n");

        printf("Your Choice: ");
        char *gameInput = (char *)malloc(sizeof(char) * STRING_SHORT);
        scanf("%s", gameInput);

        if (!(atoi(gameInput) >= 1 && atoi(gameInput) <= 4))
        {
            continue;
        }

        // Basic Attack
        if (atoi(gameInput) == 1)
        {
            if (currentPlayer == 1)
            {
                P2Health -= P1Attack;
            }
            else
            {
                P1Health -= P2Attack;
            }
        }
        // Ultimate Attack
        else if (atoi(gameInput) == 2)
        {
            if (currentPlayer == 1)
            {
                P2Health -= GetUltimateAttack(P1Attack);
                P1Health -= P1Attack;
            }
            else
            {
                P1Health -= GetUltimateAttack(P2Attack);
                P2Health -= P2Attack;
            }
        }
        else if (atoi(gameInput) == 3)
        {
            if (currentPlayer == 1)
            {
                if (P1Health >= characters[P1SelectedCharacter].health)
                {
                    PromptErrorEnter("\nCan't buy medkit, Your health is still full!!!", 1);
                    continue;
                }

                if (P1Money < MEDKIT_PRICE)
                {
                    PromptErrorEnter("\nYou don't have enough money to buy medkit!!", 1);
                    continue;
                }

                P1Money -= MEDKIT_PRICE;
                if (P1Health > characters[P1SelectedCharacter].health - MEDKIT_VALUE)
                    P1Health = characters[P1SelectedCharacter].health;
                else
                    P1Health += MEDKIT_VALUE;
                PromptSuccessEnter("Successfully healed Player 1 using medkit\n", 1);
            }
            else
            {
                if (P2Health >= characters[P2SelectedCharacter].health)
                {
                    PromptErrorEnter("\nCan't buy medkit, Your health is still full!!!", 1);
                    continue;
                }

                if (P2Money < MEDKIT_PRICE)
                {
                    PromptErrorEnter("\nYou don't have enough money to buy medkit!!", 1);
                    continue;
                }

                P2Money -= MEDKIT_PRICE;
                if (P2Health > characters[P2SelectedCharacter].health - MEDKIT_VALUE)
                    P2Health = characters[P2SelectedCharacter].health;
                else
                    P2Health += MEDKIT_VALUE;
                PromptSuccessEnter("Successfully healed Player 2 using medkit\n", 1);
            }
        }

        if (currentPlayer == 1)
            currentPlayer = 2;
        else
            currentPlayer = 1;
    }

GameOver:
    ClearScreen();
    SetColor(COLOR_RED);
    printf(
        "   ____ _____ _____ ___  ___  ____ _   _____  _____\n"
        "  / __ `/ __ `/ __ `__ \\/ _ \\/ __ \\ | / / _ \\/ ___/\n"
        " / /_/ / /_/ / / / / / /  __/ /_/ / |/ /  __/ /    \n"
        " \\__, /\\__,_/_/ /_/ /_/\\___/\\____/|___/\\___/_/     \n"
        "/____/   \n\n");
    ResetColor();

    SetColor(COLOR_GREEN);
    if (P1Health > P2Health)
    {
        printf("Player 1 (%s) Wins!!! and Player 2 (%s) Loses!!!\n\n", characters[P1SelectedCharacter].name, characters[P2SelectedCharacter].name);
    }
    else
    {
        printf("Player 2 (%s) Wins!!! and Player 1 (%s) Loses!!!\n\n", characters[P2SelectedCharacter].name, characters[P1SelectedCharacter].name);
    }
    ResetColor();

    printf("Hp left from Player 1: %d\n", P1Health);
    printf("Hp left from Player 2: %d\n", P2Health);

    printf("\n");
    PromptEnter("Press Enter to go back to main menu...", 1);
    goto GameStart;

    return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Color Macros
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define BLUE    "\033[1;34m"
#define PINK    "\033[1;35m"
#define CYAN    "\033[1;36m"
#define YELLOW  "\033[1;33m"
#define WHITE   "\033[1;37m"
#define PURPLE  "\033[1;35m"
#define RESET   "\033[0m"

// Global Variables
int unlockedSkillsCount = 0;
char currentUser[50] = "";
char acquiredSkills[10][50];

// Function Prototypes
int registerUser();
int loginUser();
int hasAnyUsers();
int isUserExists(const char *username);

void startPowerPuffQuest(int isResuming);

void saveGameState(
    int level,
    int hp,
    int maxHP,
    int attack,
    int characterChoice
);

int loadGameState(
    int *level,
    int *hp,
    int *maxHP,
    int *attack,
    int *characterChoice
);

void deleteSaveState();

int getValidInt();
void clearInputBuffer();

void showTeamDialogue(int stage);
void restArea(int *hp, int maxHP);


// =====================================================
//                                           CLEAR INPUT BUFFER
// =====================================================

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// =====================================================
//                                              GET VALID INTEGER
// =====================================================

int getValidInt() {
    int value;

    while (scanf("%d", &value) != 1) {
        clearInputBuffer();

        printf(RED"Invalid input! Please enter a valid number: "RESET);
    }

    return value;
}


// =====================================================
//                                              CHECK IF ANY USER EXISTS
// =====================================================

int hasAnyUsers() {
    FILE *fp = fopen("users.txt", "r");

    if (fp == NULL){
        return 0;
    }

    char username[50];
    char password[50];

    if (fscanf(fp, "%49s %49s",username, password) == 2){
        fclose(fp);
        return 1;
    }

    fclose(fp);
    return 0;
}


// =====================================================
//                                           CHECK IF USER EXISTS
// =====================================================

int isUserExists(const char *username) {

    FILE *fp = fopen("users.txt", "r");

    if (fp == NULL){
        return 0;
    }

    char fileUser[50];
    char filePass[50];

    while (fscanf(fp,"%49s %49s", fileUser, filePass) == 2){
        if (strcmp(username, fileUser) == 0){
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


// =====================================================
//                                          REGISTER USER
// =====================================================

int registerUser() {
    char username[50];
    char password[50];

    printf(CYAN"\nEnter a new username: "RESET);
    scanf("%49s", username);

    if (isUserExists(username)) {
        printf(RED"\nThis username already exists!\n"RESET);
        printf(YELLOW"Please try another username.\n"RESET);
        return 0;
    }

    FILE *fp = fopen("users.txt", "a");

    if (fp == NULL) {
        printf(RED"File error!\n"RESET);
        return 0;
    }

    printf(CYAN"Enter password (must be less than 49 characters): "RESET);
    scanf("%49s", password);

    fprintf(fp,"%s %s\n", username, password);
    fclose(fp);

    printf(GREEN"\nRegistration successful!\n"RESET);

    printf(YELLOW"Please login now.\n"RESET);
    return 1;
}


// =====================================================
//                                               LOGIN USER
// =====================================================

int loginUser() {
    char username[50];
    char password[50];

    char fileUser[50];
    char filePass[50];

    FILE *fp = fopen("users.txt", "r");

    if (fp == NULL){
        printf(RED"\nNo user found!\n"RESET);
        printf(YELLOW"Please register first.\n"RESET);
        return 0;
    }

    printf(CYAN"\nEnter username: "RESET);
    scanf("%49s", username);

    printf(CYAN"Enter password (must be less than 49 characters): "RESET);
    scanf("%49s", password);

    int isMatch = 0;

    while (fscanf(fp,"%49s %49s", fileUser, filePass) == 2) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0){

            isMatch = 1;

            strcpy(currentUser, username);
            break;
        }
    }

    fclose(fp);

    if (!isMatch) {
        printf(RED"\nIncorrect username or password!\n"RESET);
        return 0;
    }

    printf(GREEN"\nLogin successful!\n"RESET);
    printf(GREEN"Welcome, %s!\n"RESET,currentUser);
    return 1;
}


// =====================================================
//                                                   SAVE GAME
// =====================================================

void saveGameState(int level, int hp, int maxHP, int attack, int characterChoice){
    FILE *fp = fopen("savegame.txt", "w");

    if (fp == NULL){
        printf(RED"Failed to save the game!\n"RESET);
        return;
    }

    fprintf(fp, "%s %d %d %d %d %d %d\n",
            currentUser,
            level,
            hp,
            maxHP,
            attack,
            unlockedSkillsCount,
            characterChoice);

    fclose(fp);

    printf(GREEN"\n[Game progress has been auto-saved!]\n"RESET);
}


// =====================================================
//                                               LOAD GAME
// =====================================================

int loadGameState(
    int *level,
    int *hp,
    int *maxHP,
    int *attack,
    int *characterChoice){

    FILE *fp = fopen("savegame.txt", "r");

    if (fp == NULL){
        return 0;
    }

    char savedUser[50];

    if (fscanf(fp,
               "%49s %d %d %d %d %d %d",
               savedUser,
               level,
               hp,
               maxHP,
               attack,
               &unlockedSkillsCount,
               characterChoice) == 7){

        if (strcmp(savedUser, currentUser) == 0){
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


// =====================================================
//                                       DELETE SAVE FILE
// =====================================================

void deleteSaveState() {
    remove("savegame.txt");
}


// =====================================================
//                                               TEAM DIALOGUE
// =====================================================

void showTeamDialogue(int stage) {
    printf(PURPLE"\n--- Powerpuff Girls Conversation ---\n"RESET);

    if (stage == 1) {
        printf(PINK"Blossom: "RESET"Girls! Townsville is in danger. Let's fight together!\n");

        printf(BLUE"Bubbles: "RESET"I brought my Octi with me!\n");

        printf(GREEN"Buttercup: "RESET"Less talking, let's beat those villains!\n");
    }

    else if (stage == 2) {
        printf(PINK"Blossom: "RESET"Great fight! Let's rest at the save point.\n");

        printf(BLUE"Bubbles: "RESET"Yay! Rest time! My health is fully restored!\n");

        printf(GREEN"Buttercup: "RESET"I'm ready for the next monster!\n");
    }

    else if (stage == 3) {
        printf(PINK"Blossom: "RESET"The final boss is ahead! Get ready!\n");

        printf(BLUE"Bubbles: "RESET"We can do it!\n");

        printf(GREEN"Buttercup: "RESET"Let's finish this!\n");
    }

    printf(PURPLE"--------------------------------\n"RESET);
}


// =====================================================
//                                                   REST AREA
// =====================================================

void restArea(int *hp, int maxHP) {

    printf(CYAN"\n=========================================\n"RESET);

    printf(CYAN"          REST AREA / SAVE POINT\n"RESET);

    printf(CYAN"=========================================\n"RESET);

    showTeamDialogue(2);

    *hp = maxHP;

    printf(GREEN"\nEveryone has rested!\n"RESET);

    printf(GREEN"HP is now %d!\n"RESET, maxHP);

    printf(YELLOW"Total unlocked skills: %d\n"RESET, unlockedSkillsCount);

    printf(CYAN"Press Enter to continue..."RESET);

    clearInputBuffer();
    getchar();
}


// =====================================================
//                                           MAIN GAME CORE
// =====================================================

void startPowerPuffQuest(int isResuming) {
    int hp = 0;
    int maxHP = 0;
    int attack = 0;
    int characterChoice = 0;
    int startLevel = 0;


    // If starting a NEW GAME,
    // delete the previous save file.

    if (!isResuming) {
        deleteSaveState();
    }


    // Clear acquired skills
    memset(acquiredSkills, 0, sizeof(acquiredSkills));


    // =================================================
    //                                          10 MONSTERS
    // =================================================

    char *monsters[10] = {
        "Fuzzy Lumpkins",
        "Mojo Jojo",
        "Sedusa",
        "Gangreen Gang",
        "HIM",
        "Princess Morbucks",
        "The Amoeba Boys",
        "The Rowdyruff Boys",
        "Clone Professor",
        "Godzilla"
    };


    // =================================================
    //                                          MONSTER HP
    // =================================================

    int monsterHP[10] = {50, 70, 90, 100,120,
                                      140,160,180,210,250};

    // =================================================
    //                                  MONSTER ATTACK
    // =================================================

    int monsterAttack[10] = {12,16, 20, 24, 28,
                                             32, 36, 40, 45, 50};


    // =================================================
    // MONSTER COLORS
    // =================================================

    char *monsterColor[10] = {
        YELLOW, PURPLE, CYAN, WHITE, RED,
        BLUE, GREEN, PINK, PURPLE, RED
    };


    // =================================================
    //                                          10 SKILLS
    // =================================================

    char *skillsList[10] = {
        "Freeze Ray",
        "Sonic Scream",
        "Laser Eye",
        "Thunder Clap",
        "Super Punch",
        "Ice Storm",
        "Energy Blast",
        "Power Wave",
        "Mega Attack",
        "Ultimate Power"
    };


    // =================================================
    //                                  RESUME SAVED GAME
    // =================================================

    if (isResuming &&
        loadGameState(&startLevel, &hp, &maxHP, &attack,&characterChoice)) {

        printf(GREEN"\nResuming game from Level %d...\n"RESET, startLevel + 1);


        // Restore unlocked skills

        for (int k = 0; k < unlockedSkillsCount; k++) {
            strcpy(acquiredSkills[k], skillsList[k]);
        }
    }


    // =================================================
    //                                              NEW GAME
    // =================================================

    else {
        printf(PINK"\nChoose your Powerpuff Girl:\n"RESET);

        printf(PINK"1. Blossom (Balanced: HP 120, Attack 20)\n"RESET);

        printf(BLUE"2. Bubbles (Tank/Healer: HP 140, Attack 15)\n"RESET);

        printf(GREEN"3. Buttercup (Berserker: HP 100, Attack 25)\n"RESET);

        printf(CYAN"Choice: "RESET);
        characterChoice = getValidInt();

        switch (characterChoice) {
            case 1:
                hp = 120;
                attack = 20;

                printf(PINK"\nYou chose Blossom!\n"RESET);
                break;


            case 2:
                hp = 140;
                attack = 15;

                printf(BLUE"\nYou chose Bubbles!\n"RESET);
                break;


            case 3:
                hp = 100;
                attack = 25;

                printf(GREEN"\nYou chose Buttercup!\n"RESET);
                break;


            default:
                printf(RED"Invalid input!\n"RESET);
                printf(YELLOW"Blossom selected by default.\n"RESET);

                hp = 120;
                attack = 20;
                break;
        }


        maxHP = hp;

        unlockedSkillsCount = 0;

        showTeamDialogue(1);
    }


    // =================================================
    //                                          10 LEVEL LOOP
    // =================================================

    for (int i = startLevel; i < 10; i++) {


        // =================================================
        //                              REST AFTER EVERY 2 LEVELS
        // =================================================

        if (i > 0 && i % 2 == 0) {
            restArea(&hp, maxHP);
        }


        // =================================================
        //                                          AUTO SAVE
        // =================================================

        saveGameState(i, hp, maxHP, attack, characterChoice
        );


        // =================================================
        //                                      LEVEL DISPLAY
        // =================================================

        printf("\n%s================ LEVEL %d ================%s\n",
               monsterColor[i],
               i + 1,
               RESET);

        printf("%sMonster: %s\n",
               monsterColor[i],
               monsters[i]);

        printf("HP: %d | Attack: %d%s\n",
               monsterHP[i],
               monsterAttack[i],
               RESET);

        int currentMonsterHP = monsterHP[i];

        // =================================================
        //                                      BATTLE LOOP
        // =================================================

        while (hp > 0 && currentMonsterHP > 0) {
            // =================================================
            //                              LOW HP WARNING
            // =================================================

            if (hp <= monsterAttack[i]) {
                printf(RED"\n[WARNING] Your HP (%d) is very low!\n"RESET, hp);
                printf(RED"You should heal now!\n"RESET);
            }

            else if (hp <= maxHP / 2) {
                printf(YELLOW"\n[TIP] Your HP is below 50%%.\n"RESET);
            }


            // =================================================
            //                                      PLAYER STATUS
            // =================================================

            printf("\nYour HP: %d/%d", hp, maxHP);

            printf(" | Attack: %d\n", attack);

            printf("%s HP: %d\n", monsters[i], currentMonsterHP);


            // =================================================
            //                                      ACTION MENU
            // =================================================

            printf(YELLOW"1. Normal Attack\n"RESET);

            printf(YELLOW"2. Heal (+20 HP)\n"RESET);


            if (unlockedSkillsCount > 0) {
                printf(YELLOW"3. Use Special Skill\n"RESET);
                printf(YELLOW"4. Save and Exit\n"RESET);
            }

            else {
                printf(YELLOW"3. Save and Exit\n"RESET);
            }


            printf(CYAN"Your choice: "RESET);

            int actionChoice = getValidInt();


            // =================================================
            //                                          NORMAL ATTACK
            // =================================================

            if (actionChoice == 1) {

                currentMonsterHP -= attack;

                printf(GREEN"\nYou dealt %d damage!\n"RESET, attack);
            }


            // =================================================
            //                                                 HEAL
            // =================================================

            else if (actionChoice == 2) {
                hp += 20;

                if (hp > maxHP){
                    hp = maxHP;
                }

                printf(GREEN"\nYou healed yourself!\n"RESET);
                printf(GREEN"Current HP: %d\n"RESET, hp);
            }


            // =================================================
            //                                              SPECIAL SKILL
            // =================================================

            else if (actionChoice == 3 &&
                     unlockedSkillsCount > 0){

                printf(PURPLE"\n--- Unlocked Skills ---\n"RESET);

                for (int s = 0; s < unlockedSkillsCount; s++) {
                    printf("%d. %s\n", s + 1, acquiredSkills[s]);
                }


                printf(CYAN
                       "Choose a skill: "
                       RESET);

                int skillChoice = getValidInt();


                if (skillChoice >= 1 &&
                    skillChoice <= unlockedSkillsCount) {

                    int skillDamage = attack * 2;

                    currentMonsterHP -= skillDamage;

                    printf(PINK
                           "\nBOOM!\n"
                           RESET);

                    printf(PINK
                           "You used '%s'!\n"
                           RESET,
                           acquiredSkills[
                               skillChoice - 1
                           ]);

                    printf(PINK
                           "You dealt %d damage!\n"
                           RESET,
                           skillDamage);
                }

                else {

                    printf(RED
                           "Invalid skill! Turn wasted.\n"
                           RESET);
                }
            }


            // =================================================
            // SAVE AND EXIT
            // =================================================

            else if (
                (actionChoice == 3 &&
                 unlockedSkillsCount == 0)
                ||
                (actionChoice == 4 &&
                 unlockedSkillsCount > 0)
            ) {

                saveGameState(
                    i,
                    hp,
                    maxHP,
                    attack,
                    characterChoice
                );

                printf(YELLOW
                       "\nGame saved successfully!\n"
                       RESET);

                printf(GREEN"You can resume later.\n"RESET);
                return;
            }

            else {
                printf(RED"Invalid move! Turn wasted.\n"RESET);
            }


            // =================================================
            // MONSTER DEFEATED
            // =================================================

            if (currentMonsterHP <= 0){
                break;
            }

            // =================================================
            //                             MONSTER COUNTER ATTACK
            // =================================================

            hp -= monsterAttack[i];

            printf("%s%s counterattacks and deals %d damage!%s\n",
                   monsterColor[i],
                   monsters[i],
                   monsterAttack[i],
                   RESET);
        }


        // =================================================
        //                                             GAME OVER
        // =================================================

        if (hp <= 0){
            printf(RED"\n=========================================\n"RESET);
            printf(RED"GAME OVER!\n"RESET);
            printf(RED"You were defeated by %s.\n" RESET,monsters[i]);
            printf(RED"=========================================\n"RESET);

            deleteSaveState();
            return;
        }


        // =================================================
        //                                         LEVEL COMPLETED
        // =================================================

        printf(GREEN
               "\nLevel %d completed!\n"
               RESET,
               i + 1);


        // =================================================
        //                                      UNLOCK NEW SKILL
        // =================================================

        strcpy(
            acquiredSkills[unlockedSkillsCount],
            skillsList[i]
        );

        unlockedSkillsCount++;

        attack += 5;


        printf(CYAN
               "New skill unlocked: %s\n"
               RESET,
               skillsList[i]);

        printf(GREEN
               "Base Attack increased by +5!\n"
               RESET);


        // =================================================
        //                                      HEALTH BONUS
        // =================================================

        hp += 15;

        if (hp > maxHP)
            hp = maxHP;


        // =================================================
        //                              FINAL BOSS WARNING
        // =================================================

        if (i == 8) {

            printf(RED
                   "\nWARNING!\n"
                   RESET);

            printf(RED
                   "The final boss is coming!\n"
                   RESET);

            printf(RED
                   "Prepare yourself for Godzilla!\n"
                   RESET);
        }
    }


    // =================================================
    //                                       FINAL VICTORY
    // =================================================

    showTeamDialogue(3);

    printf(PINK
           "\n=======================================================\n"
           RESET);

    printf(PINK
           "CONGRATULATIONS!\n"
           RESET);

    printf(PINK
           "You defeated all 10 villains!\n"
           RESET);

    printf(PINK
           "Townsville is safe!\n"
           RESET);

    printf(PINK
           "You completed PowerPuff Quest!\n"
           RESET);

    printf(PINK
           "=======================================================\n"
           RESET);


    // Delete save after completing the game

    deleteSaveState();
}

// =====================================================
//                                              MAIN FUNCTION
// =====================================================
int main() {
    int choice;
    int isLoggedIn = 0;
    int isRegistered = 0;

    while (1) {
        printf(PINK "\n===== PowerPuff Quest =====\n" RESET);
        // =================================================
        // NOT LOGGED IN
        // =================================================

        if (!isLoggedIn) {

            // Registration has NOT been done in this session
            if (!isRegistered) {
                printf(YELLOW "1. Register\n" RESET);
                printf(YELLOW "2. Login\n" RESET);
                printf(YELLOW "3. Exit\n" RESET);

                printf(CYAN "Choice: " RESET);
                choice = getValidInt();

                switch (choice) {
                    case 1:
                        if (registerUser()) {
                            isRegistered = 1;
                        }
                        break;

                    case 2:
                        if (loginUser()) {
                            isLoggedIn = 1;
                        }
                        break;

                    case 3:
                        printf(GREEN "Thank you!\n" RESET);
                        return 0;

                    default:
                        printf(RED "Invalid input!\n" RESET);
                }
            }

            // =================================================
            // REGISTRATION ALREADY DONE
            // =================================================

            else {
                printf(YELLOW "1. Login\n" RESET);
                printf(YELLOW "2. Exit\n" RESET);

                printf(CYAN "Choice: " RESET);
                choice = getValidInt();

                switch (choice) {
                    case 1:
                        if (loginUser()) {
                            isLoggedIn = 1;
                        }
                        break;

                    case 2:
                        printf(GREEN "Thank you!\n" RESET);
                        return 0;


                    default:
                        printf(RED "Invalid input!\n" RESET);
                }
            }
        }

        // =================================================
        // LOGGED IN
        // =================================================

        else {
            int level;
            int hp;
            int maxHP;
            int attack;
            int characterChoice;

            int hasSave = loadGameState(
                &level,
                &hp,
                &maxHP,
                &attack,
                &characterChoice
            );

            printf(GREEN"\nLogged in as: %s\n"RESET,currentUser);

            printf(YELLOW "1. Start New Game\n" RESET);

            if (hasSave) {
                printf(YELLOW
                       "2. Resume Saved Game (Level %d)\n"
                       RESET,
                       level + 1);

                printf(YELLOW "3. Logout\n" RESET);
                printf(YELLOW "4. Exit Game\n" RESET);
            }

            else {
                printf(YELLOW "2. Logout\n" RESET);
                printf(YELLOW "3. Exit Game\n" RESET);
            }

            printf(CYAN "Choice: " RESET);
            choice = getValidInt();

            // =================================================
            // SAVE EXISTS
            // =================================================

            if (hasSave) {
                switch (choice) {
                    case 1:
                        startPowerPuffQuest(0);
                        break;

                    case 2:
                        startPowerPuffQuest(1);
                        break;

                    case 3:
                        isLoggedIn = 0;
                        currentUser[0] = '\0';

                        printf(GREEN"Successfully logged out.\n"RESET);
                        break;

                    case 4:
                        printf(GREEN"Thank you for playing!\n"RESET);
                        return 0;

                    default:
                        printf(RED"Invalid input!\n"RESET);
                }
            }


            // =================================================
            // NO SAVE EXISTS
            // =================================================

            else {
                switch (choice) {
                    case 1:
                        startPowerPuffQuest(0);
                        break;

                    case 2:
                        isLoggedIn = 0;
                        currentUser[0] = '\0';

                        printf(GREEN"Successfully logged out.\n"RESET);
                        break;

                    case 3:
                        printf(GREEN"Thank you for playing!\n"RESET);
                        return 0;

                    default:
                        printf(RED"Invalid input!\n"RESET);
                }
            }
        }
    }

    return 0;
}

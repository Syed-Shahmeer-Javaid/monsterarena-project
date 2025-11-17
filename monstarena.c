#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//  function enables colored text and cursor control.
void enableANSI() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0; 
    GetConsoleMode(h, &mode);
    mode |= 0x0004;
    SetConsoleMode(h, mode);
}
   // This function prints a HP bar.
void healthBar(int hp, int maxHp) {
     const int BAR_WIDTH = 40; 
      int filled = (hp * BAR_WIDTH) / maxHp;  // formula to calculate how many * would be printed
         printf("[");
       for (int i = 0; i < BAR_WIDTH; i++) {
          if(i < filled){
            printf("*");
          }
         else{
            printf(" ");
         }
     }
     printf("] %d/%d\n",hp,maxHp);
}

void clearBattleSection() {
    printf("\033[6;1H");  //moves the cursor to row 6 and column 1 
    printf("\033[J");    //clear screen from the cursor downwards
}

// function reads and cross check an integer for input validation,if the user types invalid input it returns -1.

int Input() {
    int value;
    char extra;
    int inputValid = scanf(" %d%c", &value, &extra); 
   // scanf will read integer first then will find new space.
     if (inputValid != 2 || extra != '\n') {
          while (getchar() != '\n'); //clear unwanted input
          return -1; //show there is error in input
    }
    return value;
    
}

int main() {
 enableANSI(); // allow ANSI cursor control
 srand(time(NULL)); // random number seed with time
 int playerMaxHP = 50; // Player max HP 
 int baseDamage = 10;  // Player basic attack strength
 int extraBoost = 15;  // Bonus attack after perfect block
 const char *enemyNames[] = {"Goblin", "Silkfang", "Dragon"};
 int enemyMaxHP[] = {50, 80, 130};  //Monster Names
 int enemyDmg[] = {12, 18, 25};   // Attack range

    printf("\033[2J\033[H");   //Clear the entire screen and move cursor to top-left.
     //TITLES
    printf("\n=== WELCOME TO MONSTER RPG ===\n"); 
    printf("---------------------------------\n");
    printf("          PREPARE YOURSELF\n");
    printf("---------------------------------\n");
    printf(" Defeat the Goblin, Silkfang, and Dragon!\n\n");
 
        //lvl = 0 denotes Goblin,lvl = 1 denotes Silkfang,lvl = 2 denotes Dragon 
    for (int level = 0; level < 3; level++) {
        int retryLevel = 1;   // retry = 1 means enter atleast once in loop & starts replaying if user enters 1 later on.
      //retry loop starts
        while (retryLevel == 1) {
       int playerHP = playerMaxHP;   // reset player HP to max HP at start
        int potions = 3;              // initialize potions
          int bonusNextAttack = 0;       // no attack boost at start
        int monsterHP = enemyMaxHP[level];  // monster HP at start = its level’s max HP.
         printf("A %s appears!\n", enemyNames[level]);
         
        // loop will execute if Player and Monster both are alive.
        
        //Battle while loop starts
            while (playerHP > 0 && monsterHP > 0) {
             // Calling bar function to draw HP bars
         printf("\nPlayer HP: ");
         healthBar(playerHP, playerMaxHP);

        printf("%s HP: ", enemyNames[level]);
        healthBar(monsterHP, enemyMaxHP[level]);
            printf("\n1) Attack   2) Defend   3) Potion (%d)\n", potions);
            printf("Choose action: ");
    
            //options for Player
                
            int choice = Input();   //checking input
            int enemyHit = (rand() % enemyDmg[level]) + 5;  // monster damage 
           int dealt = 0;   // player's damage

               //PLAYER'S ACTIONS
                // 1)ATTACK
                if (choice == 1) {   
                     dealt = (rand() % 15) + baseDamage + bonusNextAttack; //dealt
                    monsterHP = monsterHP - dealt;
                     printf("\nYou strike for %d damage!\n", dealt);
                     Beep(900, 200);
                    bonusNextAttack = 0;
                }
                // 2)DEFEND
                else if (choice == 2) { 
                    int chance = rand() % 100; // random chance number between 0–99 
                      if (chance < 20) {  
                         printf("\nPerfect block! Your next attack gets stronger.\n");
                         bonusNextAttack = extraBoost;
                        enemyHit = 0;
                    }
                    else if (chance < 80) {
                         printf("\nYou block the attack.\n");
                         enemyHit = 0;
                    }
                    else {
                        printf("\nYou failed to block!\n");
                    }
                }
                  // 3)USE POTION
                else if (choice == 3) { 
                      if (potions == 0) {
                         printf("\nYou're out of potions!\n");
                        Beep(600, 100);
                         enemyHit=0;
                    }
                    else if (playerHP == playerMaxHP) {
                        printf("\nYou're already at full health.\n");
                         Beep(600, 100);
                         enemyHit=0;
                    }
                    else {
                        playerHP =   playerHP + 30;
                         if (playerHP > playerMaxHP)
                            playerHP = playerMaxHP; 
                            
                         potions--;
                          enemyHit=0;
                         printf("\nYou drink a potion. (%d left)\n", potions);
                        Beep(600, 100);
                    }
                }
                //INVALID CHOICE
                else {   
                    printf("Invalid choice.\n");
                    Sleep(800);
                      clearBattleSection();
                    continue;   // skip monster turn
                }
                 //MONSTER'S TURN FOR ATTACK
                Sleep(450);
                if (monsterHP > 0 && enemyHit > 0) {
                     playerHP = playerHP - enemyHit;
                    printf("\n%s hits you for %d damage.\n", enemyNames[level], enemyHit);
                     Beep(300, 150);
                }
                Sleep(1500);
                clearBattleSection();
            }   // end of battle while loop 
            
             //PLAYER DIES
            if (playerHP <= 0) {
                while (1) {
                     printf("\nYou were defeated by the %s.\n", enemyNames[level]);
                    printf("1) Retry Level   2) Exit Game\n");
                    printf("Choice: ");
                     retryLevel = Input();
                    if (retryLevel == 1 || retryLevel == 2)
                        break;      //if user enter 1 or 2 only then exit this loop
                    else
                        printf("Invalid option. Enter 1 or 2.\n"); 
                }
                if (retryLevel == 2)
                     return 0; //exit from game entirely
                    
                printf("\nRestarting level...\n"); //if user enters 1
                Sleep(1000);
                 clearBattleSection();
            }  
             //PLAYER WINS
            else {
                printf("\nYou defeated the %s!\n\n", enemyNames[level]);
                // Level-up rewards
                baseDamage += 5;
                 extraBoost += 5;

                retryLevel = 0;  // move to next level for next monster   
            }
        } //end retry loop but go to level loop
    }      //end level loop

    printf("Congratulations! You defeated all monsters!\n");
     return 0;
}
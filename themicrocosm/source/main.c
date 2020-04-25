#include <stdio.h>
#include <stdlib.h> /*for system()*/
#include <time.h> /*for system()*/
#include <string.h>
#include <unistd.h>

//function to clear screen
void clr(void) {
  system("clear");
  printf(">>> THE MICROCOSM\n\n");
}

// function to sleep less than 1 sec
int smallsleep (int ms) {
  useconds_t usec;
  int r;

  usec = (useconds_t) ms *1000;
  r = usleep (usec);
  return (r);
  
}

// print text char by char
void slowprint(char text[]) {
  for(int c = 0; c < 20; c++) {
    switch(text[c]) {
      case ' ':
        smallsleep(10);
        break;
      case '\0':
        smallsleep(1);
        break;
      default:
        smallsleep(50);
    }
    putchar(text[c]);
    fflush(stdout);
  }
}


int trigger_glitch(void) {
  return ( (int) 1+ rand()%2);
}

void room(int room_no);

int interact_select(int room_no, int level, char *prevText, char *resText, int *tokens, int *glitch);

void interact_result(int room_no, int level, int sel, char *textR);


int main(void) {
  while(1)
  {  
    

    srand(time(0));
    
    char name[20];
    char *prevText = malloc(50*sizeof(char));
    char *resText = malloc(50*sizeof(char));
    char roomString[20] = "";
    
    int min, sec, choice, pglitch, psel;
    int sel = 0;
    int *tokens = malloc(sizeof(int));
    int *glitch = malloc(sizeof(int));
    int room_no= 1;
    
    time_t start, end;
    time(&start);

    char game[20] = "THE MICROCOSM\n\n";
    printf("\n");

    int chosen[3] = {0,0,0};

    printf(">>> ");

    // title print
    slowprint(game);
    
    sprintf(prevText, "text");

    sprintf(resText, "res");
    printf("\n");
    // printf(">>> THE MICROCOSM\n\n");
    // printf("\n\n%s\n\n", prevText);
    // printf("\n\n%s\n\n", resText);

    // main loop of game rooms
    while(room_no < 12)
    {  
      printf(">>> ");
      sprintf(roomString, "ROOM %d", room_no);
      slowprint(roomString);
      sleep(2);
      printf("\n\n");

      // loop for singular room.
      while (*tokens != 3){
        pglitch = *glitch;
        
        // user interact with menu
        sel = interact_select(room_no, 1, prevText, resText, tokens, glitch);
        
        // mark location as visited if visited
        if(sel!= -1 && *glitch == pglitch){
          if(chosen[sel-1] == 1) {
            sel = -1;
            printf("\n\nAlready visited.\n\n");
          }
          chosen[sel-1] = 1;
          chosen[0] = 0;
        }
        // 
        if (*tokens == 3) {
          break;
        }
        // meter for tokens and glitches
        printf("Tokens: [");
        int i = 1;
        for(int j = 0; j < 3; j++){
          printf("%s", i++ <= *tokens ? "=" : " ");
        }
        printf("]\n");
        printf("Glitch: [");
        i = 1;
        for(int j = 0; j < 5; j++){
          printf("%s", i++ <= *glitch ? "=" : " ");
        }
        printf("]\n\n");
        // smallsleep(1000);
        if (*glitch >= 5) {
          tokens = 0;
          *glitch = 0;
          for(int k = 0; k < 3; k++) {
            chosen[k] = 0;
          }
          clr();
          printf("\nMax Glitch! Reset initiated.\n");
          sleep(2);
          clr();
        }

        while(sel == -1) {
          sprintf(prevText, "text");

          sprintf(resText, "res");
          
          


          sel = interact_select(room_no, 1, prevText, resText, tokens, glitch);
          // pglitch = *glitch;
          if(sel != psel && *glitch != pglitch) {
            printf("\n\nAlready visited.\n\n");
            sel = -1;
            continue;
          }else {
            if(sel!= -1 && *glitch == pglitch){
              if(chosen[sel-1] == 1) {
                sel = -1;
                printf("\n\nAlready visited.\n\n");
              }
              chosen[sel-1] = 1;
              chosen[0] = 0;
            }
            psel = sel;
          }
        }
        
      }
      printf("\n----ROOM %d ESCAPED!----\n\n", room_no++);
      *tokens = 0;
      *glitch = 0;
      sprintf(prevText, "text");

      sprintf(resText, "res");
      for(int k = 0; k < 3; k++) {
        chosen[k] = 0;
      }
    }
    time(&end);
    min = (int) (end - start) / 60;
    sec = (int) (end - start) % 60;
    printf("\n\nGame Complete! Time taken: %dm %ds\n\n", min, sec);


    printf("\nThe truth may be too hard to bear. Press 0 to wipe your memory and reset. Press 1 to join the collective.\n\nYour choice: ");
    scanf("%d", &choice);

    if(choice) {
      exit(0);
    }}



  exit(0);
}

void room(int room_no) {
  double glitch = 0;

  glitch = glitch + trigger_glitch();
  printf("%.2f\n", glitch);
}


int interact_select(int room_no, int level, char *prevText, char *resText, int *tokens, int *glitch) {
  int sel;
  FILE *fp;
  char textT[50], textR[50];
  char display_msg[50];
  int bufferLength = 255;
  char buffer[bufferLength];
  int i = 0;
  int reslvl;
  int ans=0;


  sprintf(textT, "room%d/%s",room_no, prevText);
  sprintf(textR, "room%d/%s",room_no, resText);
  
  fp = fopen(textT, "r");

  if(strncmp(prevText,"text1", 100) != 0 || *tokens >= 2)
  {  
    int trig = 2;
    while(fgets(buffer, bufferLength, fp)) {
      if(strncmp(prevText,"text1", 100)==0 && trig){
        if(trig == 1) {
          printf("%s\n",buffer);
        }
        trig--;
        continue;
      }
      
      if(trig >= 1) {
          printf("%s\n",buffer);
          trig=trig ? trig-1 : trig;
          continue;
      }
      trig=trig ? trig-1 : trig;
      if(i==3) {
        ans = atoi(buffer);
        break;
      }
      printf("(%d) %s\n", ++i, buffer);
    }

    // printf("\nans: %d\n", ans);
    printf("\nInteract with: ");
    smallsleep(200);
    scanf("%d", &sel);
  }else {
    printf("The door seems to be welded shut. Maybe you need the other tokens first.\n\n");
    printf("----Returning to Location Menu----\n\n");
    smallsleep(500);
    return -1;
  }
  clr();
  if(ans == 0) {
    interact_result(room_no, level, sel, resText);
  }else {
    
    if(ans==sel) {
      (*tokens)++;
      printf("Correct!\n1 Token Earned!\n");
      smallsleep(500);
      return -1;
    }else{
      printf("Wrong Answer.\nGlitch Experienced!\n\n");

      smallsleep(500);
      printf("----Returning to Location Menu----\n\n");
      smallsleep(500);
      *glitch = *glitch + trigger_glitch();
      return -1;
    }
  }
  
  // printf("\nrt-%s\n\n", textR);

  sprintf(resText, "%s%d",resText, sel);
  // printf("\nrtaf-%s\n\n", textR);

  // printf("\nraa%s\n\n",resText);
 
  sprintf(prevText, "%s%d",prevText, sel);
  
  return sel;
}

void interact_result(int room_no, int level, int sel, char *textR) {
  FILE *fp;
  char text[50];
  char display_msg[50];
  int bufferLength = 255;
  char buffer[bufferLength];
  int i = 0;

  sprintf(text, "room%d/%s", room_no, textR);

  fp = fopen(text, "r");

  while(fgets(buffer, bufferLength, fp)) {
    if(++i == sel)
      printf("%s\n", buffer);
  }
}

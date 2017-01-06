#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int settings(const int val)
{
  /*Init calls:
   *1- check whether config file exists/is valid. If it's missing/corrupted then
   *generate new one via recursive call with value 2.
  */
  FILE * fp;
  static int i,ai_level;
  static int counter=0;
  static char lang[1000],*buffer;
  char p1[30],ai_level_buff[30];
  char p2[30];
  char *ptr_array[30]={&lang[0],&ai_level_buff[0],&p1[0],&p2[0]};
  const char *constant_text[30]={"Language","AI_level","Player1_name","Player2_name"};
  const char *options[30]={"en-gb","pl-pl"};
  ssize_t read;
  size_t len = 0;
  if(val==1){
  if((fp = fopen("settings.ini","r"))){
    for(i=0;i<4;i++)
      if ((read=getline(&buffer, &len, fp))>0){
        if((p1==ptr_array[i])||(p2==ptr_array[i])){
          if(strlen(buffer)>sizeof(p1)){
            printf("Za dużo znaków!");
            break;
            break;
          }
        }
        strcpy(ptr_array[i],buffer);
        buffer= strtok(ptr_array[i],":");
        if((strcmp(constant_text[i],ptr_array[i]))!=0){
          printf("Your configuration file is corrupted!'%s' is not equal to '%s'! Your config will be restored to default.\n",ptr_array[i],constant_text[i]);
          settings(3);
          return -1;
        }
        printf("%s",buffer);
        buffer= strtok(NULL,":");
        buffer[strcspn(buffer, "\n")] = 0;

        strcpy(ptr_array[i],buffer);
        printf(" %s\n",ptr_array[i]);
        if(ai_level_buff==ptr_array[i]){
          ai_level=strtol(ai_level_buff,NULL,10);
        }
      }
    if(((strcmp(lang,options[0])!=0)&&(strcmp(lang,options[1])!=0))||(ai_level>5||ai_level<0))
      printf("Your config is incorrect. Verify whether you didn't misspell any options such as language or AI_level. ");
  }else{
    printf("Your configuration file doesn't exist!\n");
    settings(3);
  }
} else  if(val==2){
    fp=fopen("settings.ini","wb");
    fprintf(fp,"Language:en-gb\n"
    "AI_level:2\n"
    "Player1_name:Peter\n"
    "Player2_name:Michael");
    fclose(fp);
    settings(1);
  }

counter++;
  return 0;
}

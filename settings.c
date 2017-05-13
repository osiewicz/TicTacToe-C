#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* settings(const int val)
{
  /*Init calls:
   *1- check whether config file exists/is valid. If it's missing/corrupted then
   *generate new one via recursive call with value -1.
  */
  FILE * fp;
  static int i,ai_level;
  static char lang[100],*buffer;
  char p1[100],ai_level_buff[100],p2[100];
  char *settings_array[30]={&lang[0],&ai_level_buff[0],&p1[0],&p2[0]};
  const char *settings_labels[30]={"Language","AI_level","Player1_name","Player2_name"};
  const char *languages[30]={"en-gb","pl-pl"};
  ssize_t read;
  size_t len=0;
  if(val>=1&&val<=5){
    if((fp = fopen("settings.ini","r"))){
      for(i=0;i<4;i++){
        if ((read=getline(&buffer, &len, fp))>0){
          strcpy(settings_array[i],buffer);
          buffer= strtok(settings_array[i],":");
          if((strcmp(settings_labels[i],settings_array[i]))!=0){
            printf("Your configuration file is corrupted!'%s' is not equal to '%s'! Your config will be restored to default.\n",settings_array[i],settings_labels[i]);
            fclose(fp);
            settings(-1);
          }
          buffer= strtok(NULL,":");
          buffer[strcspn(buffer, "\n")] = 0;
          strcpy(settings_array[i],buffer);
          if(ai_level_buff==settings_array[i]){
            ai_level=strtol(ai_level_buff,NULL,10);
            *settings_array[i]=ai_level;
          }
        }
      }
      if(((strcmp(lang,languages[0])!=0)&&(strcmp(lang,languages[1])!=0))||(ai_level>5||ai_level<0)){
        printf("Your config is incorrect. Generating new one...\n");
        fclose(fp);
        settings(-1);
      }
      if(val>1&&val<=5){
        return settings_array[val-2];
      }
      }else{
        printf("Your configuration file doesn't exist!\n");
        settings(-1);
      }
  } else  if(val==-1){
    fp=fopen("settings.ini","wb");
    fprintf(fp,"Language:en-gb\n"
    "AI_level:2\n"
    "Player1_name:Peter\n"
    "Player2_name:Michael");
    fclose(fp);
    return 0;
  }
  if(fp)
  	fclose(fp);
  return 0;
}

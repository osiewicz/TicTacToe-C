#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

void eprintf(char *fmt, ...);


const char **strings(char *language)
{
  char *newline_char;
  static char *(text)[15];
  int i;
  FILE *fp;
  size_t t = 100;
  if (strcmp(language, "en-gb") == 0) {
    if (access( ".en-gb.lang", F_OK ) != -1) {
      fp = fopen(".en-gb.lang","r");
    } else {
      eprintf("Language pack for %s is missing!"
      " Consider using other language packs or download them from"
      " GitHub repository.",language);
    }
  } else if (strcmp(language,"pl-pl") == 0){
    if (access(".pl-pl.lang", F_OK) != -1) {
      fp=fopen(".pl-pl.lang", "r");
    } else {
      eprintf("Language pack for %s is missing!"
      " Consider using other language packs or download them from"
      " GitHub repository.", language);
    }
  } else {
    eprintf("Your package is corrupted! Please consider redownloading"
    " whole package at https://github.com/PiotrOsiewicz/TicTacToe-C\n");
  }

  if (fp) {
  	for(i=0;!feof(fp);i++){
    			if (getline(&(text)[i],&t,fp)>0){
      				newline_char=strpbrk(text[i],"\n");
      				*newline_char=0;
      				continue;
    			}
  	}
	fclose(fp);
  } else {
	eprintf("Failed to open language file");
  }
  return (const char**)text;
}
void eprintf(char *fmt, ...)
{
	va_list args;

	fflush(stdout);
	fprintf(stderr, "TicTacToe : ");
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	if (fmt[0] != '\0' && fmt[strlen(fmt) - 1]) {
		fprintf(stderr, "%s", strerror(errno));
	}
	fprintf(stderr, "\n");
	exit(2);
}

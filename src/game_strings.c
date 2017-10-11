#include <game_strings.h>

static FILE *open_lang_file(const char *language)
{
	int core_length = strlen(language);
	FILE *fp;
	char *filename = calloc(core_length + 7,sizeof(char));
	filename[0] = '.';
	strcat(filename + 1, language);
	strncat(filename, ".lang", 5);
	if (access(filename, F_OK) != -1) {
		fp = fopen(filename, "r");
		if (!fp) {
			eprintf("open_lang_file: Failed to open file .%s.lang:", language);
		}
	} else {
		eprintf("open_lang_file: File \".%s.lang\" was not found:", language);
	}
	free(filename);
	return fp;
}

int load_strings(char *language,char ***destination)
{
	char *newline_char;
	char **text=NULL;
	int i;
	FILE *fp = open_lang_file(language);
	size_t t = 0;

	if (fp) {
		for(i=0;!feof(fp);i++){
			char **temp=NULL;
			temp = realloc(text, sizeof(char*)*(i+1));
			if(!temp){
				eprintf("load_strings: Error reallocating memory: ");
			}
			text = temp;
			text[i] = NULL;
			t = 0;
			/* Getline allocates memory when passed NULL and t = 0*/
			if (getline(&(text)[i],&t,fp)>0){
				newline_char=strpbrk(text[i],"\n");
				if(newline_char != NULL){
					*newline_char=0;
				}
			}
		}
		fclose(fp);
	} else {
		eprintf("Failed to open language file");
	}
	*destination = text;
	return i;
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

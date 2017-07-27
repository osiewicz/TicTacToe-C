#include <game_strings.h>

void eprintf(char *fmt, ...);

static FILE *open_lang_file(const char *language)
{
	FILE *fp;
	char *filename = malloc(sizeof(char) * (strlen(language) + 7));
	filename[0] = '.';
	memcpy(filename + 1, language, strlen(language));
	strncat(filename + strlen(language), ".lang", 5);
	if (access(filename, F_OK) != -1) {
		fp = fopen(filename, "r");
		if (!fp) {
			eprintf("open_lang_file: Failed to open file .%s.lang:", language);
		}
	} else {
		eprintf("open_lang_file: File \".%s.lang\" was not found:", language);
	}
	return fp;
}

const char **strings(char *language)
{
	char *newline_char;
	static char *(text)[15];
	int i;
	FILE *fp = open_lang_file(language);
	size_t t = 100;

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

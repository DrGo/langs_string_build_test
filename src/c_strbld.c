// gcc -O3 -march=native -funroll-loops -Ofast c_strbld.c -o ..\exe\c_strbld.exe
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void crash(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void uintToStr(unsigned long int num, char *buf) {
    char *ptr = buf;
    
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }
    
    unsigned long int temp = num;
    while (temp > 0) {
        temp /= 10;
        ptr++;
    }
    *ptr = '\0';
    while (num > 0) {
        *(--ptr) = (num % 10) + '0';
        num /= 10;
    }
}

typedef struct StringBuilder {
  char *str;
  size_t len;
  size_t cap;
} StringBuilder;

StringBuilder *newStringBuilder() {
  StringBuilder *sb = (StringBuilder *)malloc(sizeof(StringBuilder));
  sb->len = 0;
  sb->cap = 100;
  sb->str = (char *)malloc(sb->cap);
  sb->str[0] = '\0';
  return sb;
}

void append(StringBuilder *sb, const char *newStr) {
  size_t slen = strlen(newStr);
  size_t newCap = (sb->len + slen); //  > 100? sb->len + slen: 1000;
  if (newCap > sb->cap) {
	  double growth = ((10-log(sb->len))>1.6 ? (10-log(sb->len)) : 1.6);
  	  newCap =  (size_t) newCap*growth; 
	  /*printf("growth: %f, newcap : %zu\n", growth, newCap);*/
	  char *tmp = (char *)realloc(sb->str, newCap);
	  if (tmp == NULL) {
      	crash("cannot allocate enough memory."); 
	  }
	  sb->cap = newCap;
      sb->str = tmp;
    }
  strcpy(sb->str + sb->len, newStr);
  sb->len += slen;
}

void deleteStringBuilder(StringBuilder *sb) {
  free(sb->str);
  free(sb);
}

void Test(size_t num) {
  StringBuilder *sb = newStringBuilder();
  append(sb, "");
  size_t i = 0L;
  /*printf("  C");*/

  clock_t startTime = clock();

  char buffer[30]="TEST ";
  for (i = 1L; i <= num; i++) {
	uintToStr(i, buffer+4);  
    /*sprintf(buffer, " C %zu", i);*/
    append(sb, buffer);
  }

  clock_t elapsedTime = clock() - startTime;
  int minutes = (int)(elapsedTime / (CLOCKS_PER_SEC * 60));
  int seconds = (int)((elapsedTime / CLOCKS_PER_SEC) % 60);
  int milliseconds = (int)((elapsedTime * 1000) / CLOCKS_PER_SEC);

  size_t length = strlen(sb->str);
  /*printf("  %d:%d:%d Iter %ld: Len %d\n", minutes, seconds, milliseconds, i-1,
   * length);*/
  printf("\"C\":{\n");
  printf("  \"time\": \"%d:%d:%d\",\n", minutes, seconds, milliseconds);
  printf("  \"iteration\": %ld,\n", i - 1); // Iteration i-1
  printf("  \"length\": %zu\n", length);
  printf("}\n");

  FILE *file = fopen("../out/c_output.txt", "w");
  if (file != NULL) {
    fputs(sb->str, file);
    fclose(file);
  } else {
    printf("Error saving string to file.\n");
  }

  /*
   FILE *file = fopen("out/c_output.txt", "w");
   if (file != NULL)
   {
               const size_t len = strlen(sb->str);
               const size_t chunk_size = 1024 * 20;
               for (size_t i = 0; i < len; i += chunk_size)
               {
                       size_t remaining_chars = len - i;
                       size_t chars_to_write = remaining_chars < chunk_size ?
   remaining_chars : chunk_size; if (fwrite(sb->str + i, sizeof(char),
   chars_to_write, file) != chars_to_write)
                       {
                               printf("Error writing string to file.\n");
                               break;
                       }
                       //printf("%zu ", i);
               }
               fclose(file);
   }
   else
   {
       printf("Error opening file.\n");
   }
 */
  deleteStringBuilder(sb);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <num>\n", argv[0]);
    return 1;
  }

  size_t num = atol(argv[1]);
  Test(num);
  return 0;
}

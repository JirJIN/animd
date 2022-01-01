#include "ttoa.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static int remove_nl(char *str)
{
  int endpt;

  endpt = -1;
  while (str[++endpt]);
  str[--endpt] = '\0';

  return 0;
}

/*
 * ANIMD_ttoa
 *
 * @desc
 *   Takes a .animd.txt and creates
 *   an equivalent .anim file
 * @param fpath
 *   Path to the .animd.txt file
 */
#define READ_CHAR if ((temp[++temp_index] = fgetc(in)) == EOF) { printf("Early EOF reached\n"); return -1; }
#define READ_HEADER(name) \
  temp_index = 0; \
  if ((temp[temp_index] = fgetc(in)) == '[') { \
    while (temp[temp_index] != ']') { \
      READ_CHAR; \
    } \
    temp[temp_index] = '\0'; \
  } \
  else { printf("File doesn't begin with [" #name "] (or EOF reached)\n"); return -1; } \
  READ_CHAR; /* '\n' */
#define READ_SIZE \
  if (fgets(temp, 256, in) != temp) { printf("Could not read line\n"); return -1; } \
  remove_nl(temp); \
  endpt = 0; \
  while (temp[endpt++] != ':'); \
  temp_int = atoi(&temp[endpt]);
#define WRITE(data, count) if (fwrite(data, sizeof(*data), count, out) != count) { printf("Could not write\n"); return -1;}
#define TELL(var) if ((var = ftell(out)) == -1L) { printf("Could not tell file"); return -1; }
#define SEEK(pos, enum) if (fseek(out, pos, enum)) { printf("Could not seek\n"); return -1; }
int ANIMD_ttoa(const char *fpath)
{
  FILE         *in;
  FILE         *out;
  char          temp[256];
  int           temp_index;
  char          anim_name[8];
  int32_t       temp_int;
  int           endpt;
  int32_t       animation_num;
  unsigned long animation_num_pos;
  int32_t       frames_num;
  unsigned long frames_num_pos;
  unsigned long current_pos;
  int32_t       total_size;

  if (!(in = fopen(fpath, "r"))) { printf("Could not open file: %s\n", fpath); return -1; }
  endpt = -1;
  while (fpath[++endpt]);
  endpt -= 4;
  strncat(temp, fpath, endpt);
  if (!(out = fopen(temp, "wb"))) { printf("Could not open file: %s\n", temp); return -1; }

  /* [ANIMATIONS] */
  READ_HEADER(ANIMATIONS);
 
  /* Get/'write' animation num pos */
  animation_num = 0;
  TELL(animation_num_pos);
  WRITE(&animation_num, 1);

  while (fgets(temp, 256, in) == temp) { /* Get name */
    ++animation_num;

    /* Write name */
    remove_nl(temp);
    WRITE(temp, 7);
    fputc('\0', out);
    printf("[ANIMATION]: %s\n", temp);

    /* 'Write' frames */
    frames_num = 0;
    TELL(frames_num_pos);
    WRITE(&frames_num, 1);
    
    /* Read the frames */
    temp_index = 0;
    while ((temp[temp_index] = fgetc(in)) == '>') {
      ++frames_num;
      /* Read the frame duration */
      temp_index = 0;
      temp[temp_index] = fgetc(in);
      while (temp[temp_index] != ',' && temp[temp_index] != '\n') temp[++temp_index] = fgetc(in);
      remove_nl(temp);
      temp_int = atoi(temp);
      WRITE(&temp_int, 1);
      printf("Frame duration %u\n", temp_int);
      /* Read the ids */
      for (int32_t i = 0; i < 4; ++i) {
        temp_index = 0;
        temp[temp_index] = fgetc(in);
        while (temp[temp_index] != ',' && temp[temp_index] != '\n') temp[++temp_index] = fgetc(in);
        temp[temp_index] = '\0';
        temp_int = atoi(temp);
        WRITE(&temp_int, 1);
        printf("    Writing data %u\n", temp_int);
      }
    }

    /* Update frames_num */
    TELL(current_pos);
    SEEK(frames_num_pos, SEEK_SET);
    WRITE(&frames_num, 1);
    SEEK(current_pos, SEEK_SET);
    
    if (feof(in)) break;
    if (fseek(in, -1, SEEK_CUR)) { printf("Could not seek"); return -1; } /* Unread the next animation name */
  }

  SEEK(animation_num_pos, SEEK_SET);
  WRITE(&animation_num, 1);

  fclose(out);
  fclose(in);

  return 0;
}

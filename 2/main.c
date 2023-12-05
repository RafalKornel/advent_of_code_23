
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "input.txt"

char *colors[3] = {
    "red",
    "green",
    "blue"};

typedef int Draw[3];

typedef struct
{
  int id;
  int draws_count;
  Draw *draws;
} Game;

int convert(char c)
{
  int char_int = c - '0';
  int result = char_int;

  return result;
}

void print_game(const Game *game)
{
  printf("Game: id: %d\n", game->id);
  for (int i = 0; i < game->draws_count; i++)
  {
    printf("r: %d, g: %d, b: %d\n", game->draws[i][0], game->draws[i][1], game->draws[i][2]);
  }
}

int extract_int(char *line, char start, char end)
{
  char *id_start = strchr(line, start);

  if (id_start == NULL)
  {
    exit(EXIT_FAILURE);
  }

  char *id_end = strchr(id_start + 1, end);

  if (id_end == NULL)
  {
    exit(EXIT_FAILURE);
  }

  size_t id_len = (size_t)(id_end - id_start);
  char *id_str = (char *)malloc(id_len);

  memcpy(id_str, id_start, id_len);

  int id = atoi(id_str);

  free(id_str);

  return id;
}

void extract_draws(const char *line, int *draws_count, Draw **draws)
{
  char *draws_start = strchr(line, ':');
  char *draws_end = strchr(line, '\0');

  int draws_length = (int)(draws_end - draws_start);

  char *private_line = (char *)malloc(draws_length);

  memcpy(private_line, draws_start, draws_length);

  size_t draws_size = 0;

  Draw draw = {0, 0, 0};

  Draw *draws_local = (Draw *)malloc(0);

  int cache;

  for (int i = 0; i < draws_length; i++)
  {

    char *needle = private_line + i;
    char c = private_line[i];

    if (isdigit(c))
    {
      continue;
    }
    else if (c == ';' || i == draws_length - 1)
    {
      draws_size++;

      draws_local = realloc(draws_local, draws_size * sizeof(Draw));

      if (draws_local == NULL)
      {
        exit(EXIT_FAILURE);
      }

      draws_local[draws_size - 1][0] = draw[0];
      draws_local[draws_size - 1][1] = draw[1];
      draws_local[draws_size - 1][2] = draw[2];

      draw[0] = 0;
      draw[1] = 0;
      draw[2] = 0;
    }
    else if (c == ',')
    {
      continue;
    }
    else if (c == ' ' && isdigit(private_line[i + 1]))
    {
      cache = extract_int(needle, ' ', ' ');
    }
    else
    {
      for (int j = 0; j < 3; j++)
      {
        char *color = colors[j];

        if (memcmp(color, needle, strlen(color)) == 0)
        {

          draw[j] = cache;
          cache = 0;
        }
      }
    }
  }

  *draws = draws_local;
  *draws_count = draws_size;

  free(private_line);
}

Game *parse_game(char *line)
{
  Game *game = (Game *)malloc(sizeof(Game));

  game->id = extract_int(line, ' ', ':');
  extract_draws(line, &game->draws_count, &game->draws);

  return game;
}

void first(FILE *fp)
{
  char *line = NULL;

  size_t len = 0;
  ssize_t read;

  int sum = 0;
  Draw benchmark = {12, 13, 14};

  Game *game;

  while ((read = getline(&line, &len, fp)) != -1)
  {
    game = parse_game(line);
    bool is_valid = true;

    for (int i = 0; i < game->draws_count; i++)
    {

      if (game->draws[i][0] > benchmark[0] || game->draws[i][1] > benchmark[1] || game->draws[i][2] > benchmark[2])
      {
        is_valid = false;
        break;
      }
    }

    if (is_valid)
    {
      sum += game->id;
    }
  }

  printf("Sum: %d\n", sum);

  if (line)
  {
    free(line);
  }
}

void second(FILE *fp)
{
  char *line = NULL;

  size_t len = 0;
  ssize_t read;

  int sum = 0;

  Game *game;

  while ((read = getline(&line, &len, fp)) != -1)
  {
    game = parse_game(line);
    Draw min_draw = {0, 0, 0};

    for (int i = 0; i < game->draws_count; i++)
    {

      for (int c_i = 0; c_i < 3; c_i++)
      {

        if (game->draws[i][c_i] != 0 && game->draws[i][c_i] > min_draw[c_i])
        {
          min_draw[c_i] = game->draws[i][c_i];
        }
      }
    }

    int power = min_draw[0] * min_draw[1] * min_draw[2];

    // printf("id: %d, power: %d\n", game->id, power);

    sum += power;
  }

  printf("Sum: %d\n", sum);

  if (line)
  {
    free(line);
  }
}

int main(int argc, char *argv[])
{

  FILE *fp;

  fp = fopen(FILENAME, "r");

  if (fp == NULL)
    exit(EXIT_FAILURE);

  // first(fp);
  second(fp);

  fclose(fp);

  exit(EXIT_SUCCESS);
}
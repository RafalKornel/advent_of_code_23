#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define FILENAME "input.txt"
#define buffer_size 1024

char *numbers[10] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"};

int convert(char c)
{
  int char_int = c - '0';
  int result = char_int;

  return result;
}

void set_counters(int value, int *first, int *last)
{

  if (*first == -1)
  {
    *first = value;
  }

  *last = value;
}

int first(int fd)
{

  char current_char;
  int first = -1, last = -1, sum = 0;

  while (read(fd, &current_char, 1))
  {

    if (isdigit(current_char))
    {
      set_counters(convert(current_char), &first, &last);

      continue;
    }

    if (current_char == '\n' || current_char == '\0')
    {
      sum += first * 10 + last;
      first = -1;
      last = -1;

      continue;
    }
  }

  printf("End of file. Sum: %d\n", sum);

  return 0;
}

int second(int fd)
{
  char buffer[buffer_size];

  int i = 0, sum = 0, first = -1, last = -1;
  char current_char;

  while (read(fd, &current_char, 1))
  {
    buffer[i] = current_char;

    i++;

    if (current_char != '\n' && current_char != '\0')
    {
      continue;
    }

    // end of the line

    buffer[i] = '\0';

    char *needle = buffer;

    while (*needle != '\0')
    {

      // search for string value
      for (int j = 0; j < 10; j++)
      {

        if (memcmp(numbers[j], needle, strlen(numbers[j])) == 0)
        {
          set_counters(j, &first, &last);
          printf("%d %d\n", first, last);

          break;
        }
      }

      // check for integer value
      if (isdigit(*needle))
      {
        set_counters(convert(*needle), &first, &last);
      }

      needle++;
    }

    sum += first * 10 + last;

    i = 0;
    first = -1;
    last = -1;

    continue;
  }

  printf("End of file. Sum: %d\n", sum);

  return 0;
}

int main(int argc, char *argv[])
{

  int fd = open(FILENAME, O_RDONLY);

  if (fd == -1)
  {
    perror("Cannot open file");
  }

  // first(fd);
  second(fd);

  close(fd);

  return 0;
}
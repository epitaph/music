#include <stdio.h>
#include <stddef.h>
#include <locale.h>

int main(void) {
  int pound='£';
  char pound2='£';

  wchar_t wpound=L'£';
  wchar_t const * const swpound=L"£";
  wchar_t swpound2[4];

  setlocale(LC_ALL, "");

  swpound2[0]=0xc2a3;
  swpound2[1]=0;

  printf("wchar_t size: %d\n", (int)sizeof(wchar_t));
  puts("--");
  puts(L"£");
  puts("£");
  printf("£\n");
  printf("%c\n", '£');
  printf("%lc\n", '£');
  puts("==");

  printf("pound: %08x, %c\n", pound, pound);
  printf("pound2: %08x, %c\n", (int)pound2, (int)pound2);

  printf("pound as lc: %08x, %lc\n", pound, pound);

  printf("constant: %08x, %lc\n", (int)L'£', (wchar_t)L'£');
  printf("wpound: %08x, %lc\n", (int)wpound, wpound);

  printf("swpound: %ls\n", swpound);
  printf("swpound2: %ls\n", swpound2);

  return 0;
}


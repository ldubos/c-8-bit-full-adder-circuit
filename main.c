#include <stdio.h>
#include <stdlib.h>

#define B2BP "%c%c%c%c%c%c%c%c"
#define B2B(byte) \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')
#define NAND(a, b) ~(a & b)

// SUM  = (A ⊼ B ⊼ Cin) ⊼ (A ⊼ ¬B ⊼ ¬Cin) ⊼ (¬A ⊼ B ⊼ ¬Cin) ⊼ (¬A ⊼ ¬B ⊼ Cin)
// Cout = (¬A ⊼ ¬B) ⊼ (¬A ⊼ ¬Cin) ⊼ (¬B ⊼ ¬Cin)
// SUM and Cout are stored in the same variable:
//   0000 0000
//          ^^- SUM
//          |
//          Cout
char full_adder_circuit(char a, char b, char cin) {
  char g1 = NAND(a, b);
  char g4 = NAND(NAND(a, g1), NAND(g1, b));
  char g5 = NAND(g4, cin);

  return (NAND(NAND(g4, g5), NAND(g5, cin)) & 0x01) + ((NAND(g5, g1) & 0x01) << 0x01);
}

char full_adder_circuit_8bit(char a, char b) {
  char t;
  char r = 0;

  t = full_adder_circuit(a, b, (char)0);
  r += t & 0x01;
  t = full_adder_circuit(a >> 0x01, b >> 0x01, t >> 0x01);
  r += (t & 0x01) << 0x01;
  t = full_adder_circuit(a >> 0x02, b >> 0x02, t >> 0x01);
  r += (t & 0x01) << 0x02;
  t = full_adder_circuit(a >> 0x03, b >> 0x03, t >> 0x01);
  r += (t & 0x01) << 0x03;
  t = full_adder_circuit(a >> 0x04, b >> 0x04, t >> 0x01);
  r += (t & 0x01) << 0x04;
  t = full_adder_circuit(a >> 0x05, b >> 0x05, t >> 0x01);
  r += (t & 0x01) << 0x05;
  t = full_adder_circuit(a >> 0x06, b >> 0x06, t >> 0x01);
  r += (t & 0x01) << 0x06;
  t = full_adder_circuit(a >> 0x07, b >> 0x07, t >> 0x01);
  r += (t & 0x01) << 0x07;

  return r;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: ./command a b\n");
    return 1;
  }

  int arg1 = atoi(argv[1]);
  int arg2 = atoi(argv[2]);

  if (arg1 > 255 || arg2 > 255) {
    printf("a and b must be in range of 0-255\n");
    return 1;
  }

  char a = (char)arg1;
  char b = (char)arg2;

  char r = full_adder_circuit_8bit(a, b);
  printf("   "B2BP"\n", B2B(a));
  printf(" + "B2BP"\n", B2B(b));
  printf("   --------\n");
  printf("   "B2BP"\n\n", B2B(r));

  printf("%u + %u = %u %s\n", (unsigned char)a, (unsigned char)b, (unsigned char)r, (arg1 + arg2) > 255 ? "(overflow)" : "");

  return 0;
}

#include <iostream>
int main() {
  int a = 10;
  int b;

  b = a++ + ++a; // b = 10 + 12 = 22

  std::cout << "a = " << a << std::endl; // a = 12
  std::cout << "b = " << b << std::endl; // b = 22

  return 0;
}

#include "stb_image_write.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define COLORCHANNEL 3

int main() {
  std::uint8_t *data = (std::uint8_t*) malloc(sizeof(std::uint8_t) * WIDTH * HEIGHT * COLORCHANNEL);
  
  int index = 0;

  for (int x = 0; x<WIDTH; x++) {
    for (int y = 0; y<HEIGHT; y++) {
      data[index++] = 255;   
      data[index++] = 100;
      data[index++] = 255;
    }
  }
  
  bool success = stbi_write_png("./image.png", WIDTH, HEIGHT, COLORCHANNEL, data, COLORCHANNEL * WIDTH);
  if (success) std::cout << "wrote to file";
  else std::cout << "unable to write to file";
  return 0;
}
#include <iostream>

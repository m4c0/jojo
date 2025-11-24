#pragma leco app
#pragma leco add_resource "poc.txt"

import hai;
import jojo;
import jute;
import print;

static int id = 0;

int main() {
  jojo::on_error([](void * ptr, jute::view msg) {
    if (ptr == &id) erran("Error:", msg);
  });

  // In async platforms (wasm, etc) it calls sometime in the future
  // In sync platforms (non-wasm :) it calls immediately
  jojo::read("poc.txt", &id, [](void * ptr, hai::cstr & buf) {
    if (ptr == &id) puta(buf);
  });
}

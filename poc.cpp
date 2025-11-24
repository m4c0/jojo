#pragma leco app
#pragma leco add_resource "poc.txt"

import hai;
import jojo;
import jute;
import print;

int main() {
  jojo::on_error([](void *, jute::view msg) {
    erran("Error:", msg);
  });

  // In async platforms (wasm, etc) it calls sometime in the future
  // In sync platforms (non-wasm :) it calls immediately
  jojo::read("poc.txt", nullptr, [](void * ptr, hai::cstr & buf) {
    puta(buf);
  });
}

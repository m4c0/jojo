#pragma leco app
#pragma leco add_resource "poc.txt"

import hai;
import jojo;
import jute;
import print;

static int id_a = 0;

struct b {
  const int check = 'CAFE';
} id_b;

int main() {
  // Showing multiple ways of identifying errors using a single error function.
  jojo::on_error([](void * ptr, jute::view msg) {
    if (ptr == &id_a) erran("Error from consumer A:", msg);
    if (static_cast<b *>(ptr)->check == 'CAFE') erran("Error from consumer B:", msg);
  });

  // In async platforms (wasm, etc) it calls sometime in the future
  // In sync platforms (non-wasm :) it calls immediately
  jojo::read("poc.txt", &id_a, [](void * ptr, hai::cstr & buf) {
    putan("Data from A", buf);
  });
  jojo::read("poc.txt", &id_b, [](void * ptr, hai::cstr & buf) {
    putan("Data from B", buf);
  });

  jojo::read("poca.txt", &id_a, [](void * ptr, hai::cstr & buf) {});
  jojo::read("poca.txt", &id_b, [](void * ptr, hai::cstr & buf) {});

  // Example of how to (re-)throw errors
  b id_b2 {};
  jojo::error(&id_b2, "custom error");
}

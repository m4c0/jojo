#pragma leco app
#pragma leco add_resource "poc.txt"

import hai;
import jojo;
import jute;
import print;

static void got_file(void * id, hai::array<char> & buf) {
  puta("Got", buf.size(), "bytes:", buf);
}

static void fail(void * id, jute::view msg) {
  erran("Error:", msg);
}

int main() {
  jojo::on_error(&fail);

  // In async platforms (wasm, etc) it calls sometime in the future
  // In sync platforms (non-wasm :) it calls immediately
  jojo::read("poc.txt", nullptr, &got_file);
}

#pragma leco app

import hai;
import jojo;
import jute;
import silog;

static void got_file(void * id, hai::array<char> & buf) {
  silog::log(silog::info, "Got %d bytes", buf.size());
}

static void fail(void * id, jute::view msg) {
  silog::log(silog::error, "Error: %.*s", static_cast<int>(msg.size()), msg.begin());
}

int main() {
  jojo::on_error(&fail);

  // In async platforms (wasm, etc) it calls sometime in the future
  // In sync platforms (non-wasm :) it calls immediately
  jojo::read("poc.html", nullptr, &got_file);
}

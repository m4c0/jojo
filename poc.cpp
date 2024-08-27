#pragma leco app

import hai;
import jojo;
import jute;
import silog;

static void got_file(void * id, hai::array<char> & buf) {
  silog::log(silog::info, "Got %d bytes: %*s", buf.size(), buf.size(), buf.begin());
}

static void fail(void * id, jute::view msg) {
  silog::log(silog::error, "Error: %.*s", static_cast<int>(msg.size()), msg.begin());
}

int main() {
  jojo::on_error(&fail);

  // In async platforms (wasm, etc) it calls sometime in the future
  // In sync platforms (non-wasm :) it calls immediately
  jojo::read("poc.html", nullptr, &got_file);

#ifndef LECO_TARGET_WASM
  using namespace jute::literals;
  jojo::write("out/test.txt", nullptr, "hello "_hs, [](void *) {
    jojo::append("out/test.txt", nullptr, "world!"_hs, [](void *) {
      // Another `read` example, using cstr instead of array
      jojo::read("out/test.txt", nullptr,
                 [](void *, hai::cstr & buf) { silog::log(silog::info, "Got [%s]", buf.begin()); });
    });
  });
#endif
}

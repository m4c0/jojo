#ifndef LECO_TARGET_WASM
#pragma leco tool
#endif

import hai;
import jojo;
import jute;
import silog;

static void fail(void * id, jute::view msg) {
  silog::log(silog::error, "Error: %.*s", static_cast<int>(msg.size()), msg.begin());
}

int main() {
  jojo::on_error(&fail);

  using namespace jute::literals;
  jojo::write("out/test.txt", nullptr, "hello "_hs, [](void *) {
    jojo::append("out/test.txt", nullptr, "world!"_hs, [](void *) {
      // Another `read` example, using cstr instead of array
      // TODO: fix this
      jojo::read("out/test.txt", nullptr,
                 [](void *, hai::cstr & buf) { silog::log(silog::info, "Got [%s]", buf.begin()); });
    });
  });

  silog::log(silog::info, "Read [%s]", jojo::read_cstr("out/test.txt").begin());
}

#ifndef LECO_TARGET_WASM
#pragma leco tool
#endif

import hai;
import jojo;
import jute;
import print;

static void fail(void * id, jute::view msg) { die(msg);
}

int main() {
  jojo::on_error(&fail);

  using namespace jute::literals;
  jojo::write("out/test.txt", nullptr, "hello "_hs, [](void *) {
    jojo::append("out/test.txt", nullptr, "world!"_hs, [](void *) {
      jojo::read("out/test.txt", nullptr, [](void *, hai::cstr & buf) { putln(buf); });
    });
  });

  putln(jojo::slurp("out/test.txt"));
}

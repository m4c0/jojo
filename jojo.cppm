export module jojo;
import hai;
import jute;
import silog;

namespace jojo {
  extern hai::fn<void, void *, jute::view> err_callback;

  export void on_error(hai::fn<void, void *, jute::view> callback) { err_callback = callback; }

  export void append(jute::view name, void *, jute::heap data, hai::fn<void, void *>);
  export void read(jute::view name, void *, hai::fn<void, void *, hai::array<char> &>);
  export void write(jute::view name, void *, jute::heap data, hai::fn<void, void *>);
}

module :private;

static void null_callback(void *, jute::view msg) {
  silog::log(silog::error, "Unexpected IO error: %s", msg.cstr().begin());
}

hai::fn<void, void *, jute::view> jojo::err_callback{&null_callback};

#ifdef LECO_TARGET_WASM
#pragma leco add_impl wasm
#else
#pragma leco add_impl libc
#endif


export module jojo;
import hai;
import jute;

namespace jojo {
  extern hai::fn<void, void *, jute::view> err_callback;

  export void on_error(hai::fn<void, void *, jute::view> callback) { err_callback = callback; }
  export void error(void * ptr, jute::view msg) { err_callback(ptr, msg); }

  export void read(jute::view name, void *, hai::fn<void, void *, hai::cstr &>);

#ifndef LECO_TARGET_WASM
  export void append(jute::view name, void *, jute::heap data, hai::fn<void, void *>);
  export void write(jute::view name, void *, jute::heap data, hai::fn<void, void *>);

  export hai::cstr slurp(jute::view name);

  export void append(jute::view name, jute::heap data);

  export void write(jute::view name, const void * data, unsigned size);
  export void write(jute::view name, const auto & data) { write(name, data.data(), data.size()); }

  export void readlines(jute::view name, hai::fn<void, jute::view> fn);
#endif

  void null_callback(void *, jute::view msg);
} // namespace jojo

module :private;

hai::fn<void, void *, jute::view> jojo::err_callback { &null_callback };

#ifdef LECO_TARGET_WASM
#pragma leco add_impl wasm
#else
#pragma leco add_impl libc
#endif

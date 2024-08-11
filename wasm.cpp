#define EXP(name) __attribute__((export_name(name)))

module jojo;
import vaselin;

using buf_t = hai::array<char>;
using fn_t = hai::fn<void, void *, buf_t &>;

extern "C" __attribute__((import_module("jojo"), import_name("read"))) void jojo_read(const char * name, unsigned nsz, void * ptr, unsigned idx);

static hai::array<fn_t> g_in_flights{16};
static hai::array<buf_t> g_buffers{16};

EXP("jojo_alloc") void * alloc(unsigned idx, unsigned sz) {
  auto & b = g_buffers[idx] = buf_t{sz};
  return b.begin();
}
EXP("jojo_callback") void callback(void * ptr, unsigned idx) {
  auto &fn = g_in_flights[idx];
  auto &buf = g_buffers[idx];

  fn(ptr, buf);
  fn = {};
  buf = {};
}

void jojo::read(jute::view name, void * ptr, fn_t fn) {
  for (auto i = 0; i < g_in_flights.size(); i++) {
    auto & p = g_in_flights[i];
    if (p) continue;

    p = fn;
    jojo_read(name.begin(), name.size(), ptr, i);
    return;
  }

  err_callback(ptr, "Too many open files");
}

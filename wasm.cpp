module jojo;
import vaselin;

#define EXP(name) __attribute__((export_name(name)))

extern "C" __attribute__((import_module("jojo"), import_name("read"))) void jojo_read(const char * name, unsigned nsz, void * ptr, unsigned idx);

struct slot {
  hai::cstr cstr_buf;
  hai::fn<void, void *, hai::cstr &> cstr_fn;

  hai::array<char> array_buf;
  hai::fn<void, void *, hai::array<char> &> array_fn;
};
static hai::array<slot> g_in_flights { 16 };

EXP("jojo_alloc") void * alloc(unsigned idx, unsigned sz) {
  auto & s = g_in_flights[idx];
  if (s.cstr_fn) {
    s.cstr_buf = hai::cstr { sz };
    return s.cstr_buf.begin();
  } else {
    s.array_buf = hai::array<char> { sz };
    return s.array_buf.begin();
  }
}
EXP("jojo_callback") void callback(void * ptr, unsigned idx) {
  auto &s = g_in_flights[idx];
  if (s.cstr_fn) s.cstr_fn(ptr, s.cstr_buf);
  else s.array_fn(ptr, s.array_buf);
  s = {};
}
EXP("jojo_err_callback") void err_callback(void * ptr, unsigned idx) {
  jojo::err_callback(ptr, "Request failed");
  g_in_flights[idx] = {};
}

void jojo::read(jute::view name, void * ptr, hai::fn<void, void *, hai::array<char> &> fn) {
  for (auto i = 0; i < g_in_flights.size(); i++) {
    auto & p = g_in_flights[i];
    if (p.cstr_fn || p.array_fn) continue;

    p.array_fn = fn;
    jojo_read(name.begin(), name.size(), ptr, i);
    return;
  }

  err_callback(ptr, "Too many open files");
}
void jojo::read(jute::view name, void * ptr, hai::fn<void, void *, hai::cstr &> fn) {
  for (auto i = 0; i < g_in_flights.size(); i++) {
    auto & p = g_in_flights[i];
    if (p.cstr_fn || p.array_fn) continue;

    p.cstr_fn = fn;
    jojo_read(name.begin(), name.size(), ptr, i);
    return;
  }

  err_callback(ptr, "Too many open files");
}

void jojo::null_callback(void *, jute::view msg) {
  vaselin::console_error(msg.begin(), msg.size());
  vaselin::raise_error();
}

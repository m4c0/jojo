module;
#include <errno.h>
#include <stdio.h>
#include <string.h>

module jojo;
import silog;

#ifdef LECO_TARGET_WINDOWS
static inline FILE *fopen(auto name, auto mode) {
  FILE *res;
  return ::fopen_s(&res, name, mode) ? nullptr : res;
}
#define strerror_r(err, buf, len) strerror_s(buf, len, err)
#endif

static void fail(void * ptr) {
  hai::cstr buf{1024};
  strerror_r(errno, buf.begin(), buf.size());
  jojo::err_callback(ptr, jute::view{buf});
}

void jojo::read(jute::view name, void * ptr, hai::fn<void, void *, hai::array<char> &> callback) {
  FILE * f = fopen(name.cstr().begin(), "rb");
  if (!f) return fail(ptr);

  if (-1 == fseek(f, 0, SEEK_END)) fail(ptr);

  auto sz = ftell(f);
  if (sz == -1) fail(ptr);

  if (-1 == fseek(f, 0, SEEK_SET)) fail(ptr);

  hai::array<char> buf{static_cast<unsigned>(sz)};
  if (1 != fread(buf.begin(), sz, 1, f)) fail(ptr);

  fclose(f);
  
  callback(ptr, buf);
}

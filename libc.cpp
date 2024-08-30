module;
#include <errno.h>
#include <stdio.h>
#include <string.h>

module jojo;
import silog;

#ifdef LECO_TARGET_WINDOWS
static inline FILE * fopen(auto name, auto mode) {
  FILE * res;
  return ::fopen_s(&res, name, mode) ? nullptr : res;
}
#define strerror_r(err, buf, len) strerror_s(buf, len, err)
#endif

static void fail(void * ptr) {
  hai::cstr buf { 1024 };
  strerror_r(errno, buf.begin(), buf.size());
  jojo::err_callback(ptr, jute::view { buf });
}

template <typename Buf> static void just_read(jute::view name, void * ptr, hai::fn<void, void *, Buf &> callback) {
  FILE * f = fopen(name.cstr().begin(), "rb");
  if (!f) return fail(ptr);

  if (-1 == fseek(f, 0, SEEK_END)) return fail(ptr);

  auto sz = ftell(f);
  if (sz == -1) return fail(ptr);

  if (-1 == fseek(f, 0, SEEK_SET)) return fail(ptr);

  Buf buf { static_cast<unsigned>(sz) };
  if (1 != fread(buf.begin(), sz, 1, f)) return fail(ptr);

  fclose(f);

  callback(ptr, buf);
}

void jojo::read(jute::view name, void * ptr, hai::fn<void, void *, hai::array<char> &> callback) {
  just_read(name, ptr, callback);
}
void jojo::read(jute::view name, void * ptr, hai::fn<void, void *, hai::cstr &> callback) {
  just_read(name, ptr, callback);
}

template <typename Buf> static Buf just_read(jute::view name) {
  Buf res {};
  just_read<Buf>(name, nullptr, [&](void *, Buf & r) { res = traits::move(r); });
  return res;
}
hai::array<char> jojo::read(jute::view name) { return just_read<hai::array<char>>(name); }
hai::cstr jojo::read_cstr(jute::view name) { return just_read<hai::cstr>(name); }

void jojo::write(jute::view name, void * ptr, jute::heap buf, hai::fn<void, void *> callback) {
  FILE * f = fopen(name.cstr().begin(), "wb");
  if (!f) return fail(ptr);

  if (1 != fwrite(buf.begin(), buf.size(), 1, f)) return fail(ptr);

  fclose(f);

  callback(ptr);
}

void jojo::append(jute::view name, void * ptr, jute::heap buf, hai::fn<void, void *> callback) {
  FILE * f = fopen(name.cstr().begin(), "ab");
  if (!f) return fail(ptr);

  if (1 != fwrite(buf.begin(), buf.size(), 1, f)) return fail(ptr);

  fclose(f);

  callback(ptr);
}

void jojo::write(jute::view name, jute::heap data) {
  jojo::write(name, nullptr, data, [=](void *) {});
}
void jojo::append(jute::view name, jute::heap data) {
  jojo::append(name, nullptr, data, [=](void *) {});
}

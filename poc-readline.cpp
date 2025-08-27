#ifndef LECO_TARGET_WASM
#pragma leco tool
#endif
import jojo;
import print;

void log(auto msg) { putln(msg); }
int main() {
  jojo::readlines(__FILE__, log);
}

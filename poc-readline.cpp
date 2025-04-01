#pragma leco tool
import jojo;
import print;

void log(auto msg) { putln(msg); }
int main() {
  jojo::readlines(__FILE__, log);
}

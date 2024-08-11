export module jojo;
import hai;
import jute;

export namespace jojo {
  void on_error(hai::fn<void, void *, jute::view>);

  void read(jute::view name, void *, hai::fn<void, void *, hai::array<char> &>);
}

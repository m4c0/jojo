!function() {
  leco_imports.jojo = {
    read : (nptr, nsz, ptr, idx) => {
      fetch(vaselin_tostr(nptr, nsz))
        .then(r => r.arrayBuffer())
        .then(r => {
          const p = leco_exports.jojo_alloc(idx, r.byteLength);
          vaselin_toarr(p, r.byteLength).set(new Uint8Array(r));
          leco_exports.jojo_callback(ptr, idx);
        });
    },
  };
}();

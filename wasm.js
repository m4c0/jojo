!function() {
  leco_imports.jojo = {
    read : (nptr, nsz, ptr, idx) => {
      fetch(vaselin_tostr(nptr, nsz))
        .then(r => {
          if (r.ok) return r;
          throw r;
        })
        .then(r => r.arrayBuffer())
        .then(r => {
          const p = leco_exports.jojo_alloc(idx, r.byteLength);
          vaselin_toarr(p, r.byteLength).set(new Uint8Array(r));
          leco_exports.jojo_callback(ptr, idx);
        })
        .catch(e => {
          console.error("Request failed", e);
          leco_exports.jojo_err_callback(ptr, idx);
        });
    },
  };
}();

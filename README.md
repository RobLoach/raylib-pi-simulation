# Pi Simulation with raylib

This is my attempt at calculating Pi using a method [discussed by YouTuber 3Brown1Blue](https://www.youtube.com/watch?v=6dTyOl1fmDo) with [raylib](https://www.raylib.com/) and [Physac](https://github.com/victorfisac/Physac). It's borked.

## Compilation

### Desktop

Use the following to build for desktop:

``` bash
cmake -B build
cmake --build build
```

### Web

Compiling for the web requires the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html):

``` bash
mkdir build
cd build
emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXECUTABLE_SUFFIX=".html"
emmake make
```

### LICENSE

[LICENSE](LICENSE)
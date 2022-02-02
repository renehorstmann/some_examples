# some_examples

some examples for [some](https://github.com/renehorstmann/some) framework

## Live demo
[livedemo](https://renehorstmann.github.io/some_examples) compiled with emscripten

## Examples
0. [the first rect](src/example_0.c)
1. [pointer events](src/example_1.c)
2. [animation](src/example_2.c)
3. [nuklear gui](src/example_3.c)
4. [up 'n download](src/example_4.c)
5. [text input](src/example_5.c)
6. [container](src/example_6.c)


## Install and run on Desktop
Have a look at the section of [some](https://github.com/renehorstmann/some)


## Compiling for Web
Using Emscripten https://emscripten.org/
Tested under Ubuntu and WSL Ubuntu.
You should have already cloned the project and `cd` to that dir:

- Create a sub directory to compile the website
```sh
mkdir web && cp index.html web && cp icon/* web && cd web
```

- Copy all resources, because emscripten may not be able to use `../res`
```sh
cp -r ../res .
```

- Compile
```sh
emcc -O3 \
-I../include/ \
-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s FULL_ES3=1 -s \
EXPORTED_FUNCTIONS='["_main", "_e_io_idbfs_synced", "_e_io_file_upload_done"]' \
-s EXPORTED_RUNTIME_METHODS=FS \
-s SDL2_IMAGE_FORMATS='["png"]' \
--preload-file ./res \
-s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 -s FETCH=1 \
-lidbfs.js \
-DOPTION_GLES -DOPTION_SDL -DOPTION_TTF -DOPTION_FETCH \
../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c \
-o index.js
```

- Test the website (open a browser and call localhost:8000)
```sh
python3 -m http.server --bind localhost  # [port]
```


## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the someLICENSE file for details
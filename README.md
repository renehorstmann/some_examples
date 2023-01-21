# some_examples

some examples for [some](https://github.com/renehorstmann/some) framework

## Live demo
[livedemo](https://renehorstmann.github.io/some_examples) compiled with emscripten

## Examples
0. [the first rect](src/example_0.c)
1. [pointer events](src/example_1.c)
2. [animation](src/example_2.c)
3. [nuklear gui](src/example_3.c)
4. [particles](src/example_4.c)
5. [text input](src/example_5.c)
6. [color picker](src/example_6.c)
7. [container](src/example_7.c)
8. [up 'n download](src/example_8.c)


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
-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s FULL_ES3=1 -s \
EXPORTED_FUNCTIONS='["_main", "_e_io_idbfs_synced", "_e_io_file_upload_done"]' \
-s EXPORTED_RUNTIME_METHODS=FS,ccall \
-s SDL2_IMAGE_FORMATS='["png"]' \
--preload-file ./res \
-s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 \
-lidbfs.js \
-DPLATFORM_EMSCRIPTEN -DOPTION_GLES -DOPTION_SDL \
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
    - [SDL2](https://www.libsdl.org/) (zlib License)
    - [Emscripten](emscripten.org) (MIT License)
    - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT License)
    - [curl](https://curl.se/docs/copyright.html) (MIT like License)
    - [sfd](https://github.com/rxi/sfd) for simple file dialogs (MIT License)

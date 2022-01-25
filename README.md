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


## Compiling for Web

Using Emscripten:

```sh
mkdir web && cp index.html web && cp icon/* web && cd web
```

```sh
cp ../res .
```

```sh
emcc -I../include/ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s FULL_ES3=1 -s EXPORTED_FUNCTIONS='["_main", "_e_io_idbfs_synced", "_e_io_file_upload_done"]' -s EXPORTED_RUNTIME_METHODS=FS -s SDL2_IMAGE_FORMATS='["png"]'  --preload-file ./res -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 -s FETCH=1 -lidbfs.js -DOPTION_GLES -DOPTION_SDL -DOPTION_TTF -DOPTION_FETCH ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -o index.js
```

test the website:
```sh
python3 -m http.server --bind localhost  # [port]
```

Add the following changes to the generated index.html:
```html
<style>
  #canvas {
    position: absolute;
    top: 0px;
    left: 0px;
    margin: 0px;
    width: 100%;
    height: 100%;
    overflow: hidden;
    display: block;
  }
</style>
<script>
    function set_exit_failure_error_msg() {
        var newContent = '<!DOCTYPE html><html><body style="background-color:black;"><h1 style="color:white;">Potato Browsers are not supported!</h1><p style="color:silver;">Full WebGL2.0 is needed!</p></body></html>';
        document.open();
        document.write(newContent);
        document.close();
    }
</script>
```
This will let Emscripten run in fullscreen and display an error text, if the app / game is not able to run (WebGL2.0 support missing)

## Without Cmake

Instead of cmake, the following call to gcc may work, too.

```sh
mkdir build && cd build

cp -r ../res .

gcc ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -I../include/ $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lSDL2_net -lglew32 -lopengl32 -lglu32 -DOPTION_GLEW -DOPTION_SDL -DOPTION_TTF -DOPTION_SOCKET -o some_examples
```

## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the someLICENSE file for details
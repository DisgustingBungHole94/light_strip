# HomeController Server

## What is LightStrip?

LightStrip is a program built with [libhomecontroller](https://github.com/DisgustingBungHole94/libhomecontroller) that turns a Raspberry Pi into a [HomeController](https://github.com/DisgustingBungHole94/HomeController_server)-compatible smart device that can control RGB LED strips over the internet.

## Build Instructions

Make sure you have G++, Clang, Node.js, npm, and npx installed.

```sh
git clone https://github.com/DisgustingBungHole94/light_strip.git

cd light_strip

git submodule update --init --recursive

make
```

This will automatically compile all dependencies as well.
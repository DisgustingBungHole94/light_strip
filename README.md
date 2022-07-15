# LightStrip

## What is LightStrip?

LightStrip is a program built with [libhomecontroller](https://github.com/DisgustingBungHole94/libhomecontroller) that turns a Raspberry Pi into a [HomeController](https://github.com/DisgustingBungHole94/HomeController_server)-compatible smart device that can control RGB LED strips over the internet using the GPIO pins.

## Build Instructions

Make sure you have G++, Clang, Node.js, npm, and npx installed.

```sh
git clone https://github.com/DisgustingBungHole94/light_strip.git --recursive

cd light_strip && make
```

This will automatically compile all dependencies as well.
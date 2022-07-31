# LightStrip

## What is LightStrip?

LightStrip is a program built with [libhomecontroller](https://github.com/DisgustingBungHole94/libhomecontroller) that turns a Raspberry Pi into a [HomeController](https://github.com/DisgustingBungHole94/HomeController_server)-compatible smart device that can control RGB LED strips over the internet using the GPIO pins.

## Build Instructions

Make sure you have [libhomecontroller](https://github.com/DisgustingBungHole94/libhomecontroller) installed.

```sh
git clone https://github.com/DisgustingBungHole94/light_strip.git
cd light_strip && make
```
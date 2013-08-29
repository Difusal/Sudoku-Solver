#pragma once

/* allegro includes */
#include "allegro5\allegro.h"
#include "allegro5\allegro_native_dialog.h"
#include "allegro5\allegro_primitives.h"
#include "allegro5\allegro_image.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"
#include "allegro5\allegro_audio.h"
#include "allegro5\allegro_acodec.h"

/* standard includes */
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

/* global variables */
#define ProgramTitle "!!! Specify the program name in \"stdIncludes.h\" !!!"
#define ScreenWidth 800
#define ScreenHeight 600
#define WorldBlockSize 40
const float FPS = 60.0;

/* user friendly names */
#define Black al_map_rgb(0, 0, 0)
#define DarkerGray al_map_rgb(32, 32, 32)
#define DarkGray al_map_rgb(64, 64, 64)
#define Gray al_map_rgb(128, 128, 128)
#define LightGray al_map_rgb(192, 192, 192)
#define DarkBlue al_map_rgb(0, 0, 150)
#define Blue al_map_rgb(0, 0, 255)
#define LightBlue al_map_rgb(14, 108, 150)
#define DarkGreen al_map_rgb(0, 200, 0)
#define Green al_map_rgb(0, 255, 0)
#define DarkRed al_map_rgb(200, 0, 0)
#define Red al_map_rgb(255, 0, 0)
#define Yellow al_map_rgb(255, 255, 0)
#define White al_map_rgb(255, 255, 255)

/* fonts */
#define ConsolaTTF "res//fonts//consola.ttf"
#define CalibriTTF "res//fonts//calibri.ttf"
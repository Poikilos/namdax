# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


## [Unreleased] - 2017-09-28
### Changed
- use RSound_audiere.cpp (add to "additional linker options": `audiere-config --libs` including grave accents)


## [Unreleased] - 2017-09-27
### Changed
- Rename methods for accuracy and to avoid terminology conflicts with
  Audiere:
  - `MoveSoundSource` to `MoveSoundEmitter`
  - `SoundSource` to `SoundEmitter`
  - `SourceName` to `EmitterName`
(The Following changes are for cross-platform compatibility and
modernization. See also 2017-09-27 in the bonefinity changelog.)
- Change `RSound*` files and objects to `IRSound*` files and objects,
  then make separate implementations as `RSound`, wrapped in `RSOUND_*`
  defines.
- Make separate targets for Release, Debug, and WindowsRelease
  (eliminate "default" target); with same settings as engine--see
  bonefinity changes for 2017-09-27.
  - Remove "Linker settings," "Other linker options":
    `-lmingw32  -mwindows` (move to WindowsRelease target only)
- Update syntax for current version of C++:
  - Change instances of `NULL` (and `null` as defined by
     frameworkdummy.h) to `nullptr`.
  - Explicitly cast size_type (such as capacity() of vector) to int
    before sending it to a function which takes an `int`.
- Update usage and project settings for modern SDL2:
  - Remove "main=SDL_main" from build options, compiler settings,
    and `#define` statements.
  - Remove the following from "Linker settings," "Other linker options":
    - `-lstdc++ -lSDL2main -lSDL2`
  - "Other compiler options":
    - `sdl2-config --cflags` (including grave accents)
  - Linker settings, "Other linker options":
    - `sdl2-config --libs` (including grave acceents) to "Project,"
      "build options"


## [Unreleased] - 2016-07-28
### Changes
- Show 2D cursor during pause and during ending to prevent apparent
  freeze (was not a real freeze) and make mandatory delay shorter.
- Change hero graphic.
- Make crosshairs as high as hero eye (change it to some better location
  later).


## [Unreleased] - 2016-07-25
### Added
- Create global function DoCollision for all collisions (instead of
  separate if statements in each shot hitting alien, shot hitting hero,
  and alien hitting hero condition)

### Changed
- Change `gbSymbolShield` to `*shield_symbol_image_ptr`.
- Change `gbSymbolBossHealth` to `*boss_health_symbol_image_ptr`.
- Replace Anim members
```
GBuffer* gbarrAnim;
GBuffer gbFrame;
```
with
```
vector<GBuffer*> frame_ptrs;
GBuffer* frame_ptr;
```

### Removed
- Eliminate temporary `GBuffer` (though using existing data buffer) from
  `RawOverlayNoClipToBig`.
- Remove `lFramesCached` variable (make Anim frame count always have 1:1
  relationship with image sequence).


## [Unreleased] - 2016-07-24
### Changed
- DrawToLargerWithoutCropElseCancel can now handle mismatched bitdepth
  and any images 16-bit or higher (will fail if alpha is used and
  source is less than 32-bit)
- Give a sequential number to each `InitNull` call for `GBuffer` and
  `Anim` (This helped track down the `Dispose` crash below).

### Fixed
- Fix "not all ... loaded" errors (`iFramesShotShadow` was used instead
  of `iFramesBurn` and `iFramesExpArt`.
- Change `arrpentAlien` into a `std::vector` (fix issue where freeing
  alien array crashes program for unknown reason).
- Name unnamed `GBuffer` objects (sFile) and output value of sFile to
  log upon unload to aid debugging (such as by prepending note in
  `Anim::Copy`, `MaskFromValue`; and automatically naming each `GBuffer`
  frame when using `SplitFromImage32`).
- long-standing animGameScreen Dispose crash [inside it's SafeFree call]


## [Unreleased] - 2014-10-24
### Fixed
- (Use atexit and SDL_Quit properly. Now program has single exit
  point after main) hang upon quit (while showing press fire to
  quit).


## [Unreleased] - 2014-09-23
### Fixed
- redundant and missing boundary checks for entity drawing method
  (`GBuffer_FX_Scaled`)
- misplaced shadows (too far down)


## [Unreleased] - 2014-09-22
### Changed
- Use 2D aiming and 2D flyer movement.


## [Unreleased] - 2014-06-29
### Changed
- For the order of parameters in `DrawExclusiveRect` to use `int right`,
  `int bottom` instead of `int bottom`, `int right`, to avoid
  inconsistency (compared to the order of ordered pairs).

### Removed
- overscan buffer and related `SCREEN_WIDTH`, `SCREEN_HEIGHT`,
  `BUFFER_WIDTH`, `BUFFER_HEIGHT`, `SCREEN_OFFSET_X`,
  `SCREEN_OFFSET_Y`, `FSCREEN_OFFSET_X`, `FSCREEN_OFFSET_Y` defines


## [Unreleased] - 2014-06-13
- Port from SDL-1.2 to SDL2.


## [Unreleased] - 2011-12-13
### Fixed
- (CppCheck) Change old casts (using Entity pointers formerly void
  pointers) from: `delete (Entity*)arrpentAlien[iAliensNow];` etc to
  `delete arrpentAlien[iAliensNow];` etc.


## [Unreleased] - 2009-09-06
### Added
- MultiThreading (to work around SDL Mixer crashes)


## [Unreleased] - 2009-09-03
### Fixed
- Scrolling Text Intro (TargaToTarga32_Alpha_Clipped function):
  - crash
  - redundant pointer multiplication of `unsigned __int32*` and
    redundant `unsigned __int32` pixel variable replaced with
    `*(unsigned
    __int32*)(lpbyDestNow-4)=_RGB32BIT(255,red,green,blue);`
  - removed unneccessary intermediate variables and `unsigned __int32*`
    pixel building macro in favor of direct value assignment and
    putting most convenient (even if inconsistent) value into
    destination alpha channel
  - removed unneccessary pointer addition in favor of adding
    bytes-per-pixel except when pixel needs to be drawn with alpha
- occurances of 256 in alpha formulas (replaced with 255)


## [Unreleased] - 2008
###
- Randomly choose from multiple burn mark shapes (`burn*.tga`).
- Improve area backgrounds.
- Separate "intro-text-back.tga" and "intro-text.tga" to allow
  scrolling.

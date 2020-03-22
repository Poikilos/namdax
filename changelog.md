# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


## [Unreleased] - 2011-12-31
(crossplatform branch)
### Changed
- Rename `DPOLAROFRECT` and `FPOLAROFRECT` to `RConvert_RectToPolar`.


## [Unreleased] - 2011-12-29
(crossplatform branch)
### Fixed
- (major fix!) changed "else {" to "if (bHitMinX||bHitMaxX||bHitMinY||bHitMaxY) {" for hit edge in case hit both edge AND bottom!
- (major fix!) changed "if (rSecondsShootDelay<Base_r0)" to "if (rSecondsShootDelay<=Base_r0)"
- changed
  `(Base_GetSeconds()-rSecLastRefresh)`
  to
  `Base_SecondsSince(rSecLastRefresh)`


## [Unreleased] - 2011-12-24
(crossplatform branch)
### Fixed
- Manually pull the fixed `Point2dFrom3dWithScreenOffset` (with a
  return) from the master (SDL) branch.
- Change `for (iNow=0; iNow<iSoundarrLen; iNow) {` to
  `for (iNow=0; iNow<iSoundarrLen; iNow++) {`
- Fix `PointIsInView` (A `>=` case did nothing)!
- Change the `#include`s in crossplatform branch-specific code to use
  new file naming (already done in bonefinity--See the Changelog there).
- Fix issues preventing compiling.
- Fix c-style pointer cast warnings no longer need "(Entity*)" cast
  since all objects are entities.
- Add missing line:
  `LoadImage(gbIntroTextBack, "images/intro-text-back.tga");`
- Add a return line to `Base_SecondsSince`!

### Changed
- Capitalize X,Y,Z properties.


## [Unreleased] - 2011-12-23
(crossplatform branch)
(see DXManSDL)


## [Unreleased] - 2009-09
(crossplatform branch)
(see also: DXManSDL 2009-09-03)
### Fixed
- Improve font.tga (resize & move characters to keep outlines from
  crossing divisions)
- Fixed infinite recursion in `Mass3D::ToString` (Replace ambiguous
  `ToString` calls with `ExpertMultimediaBase::ToString`.
- Improve "area0000.tga" and "area0001.tga".
- added "no checkpoint reached" to gamescreenarea0000.tga
- Refactor to comply with C++ standards (used extern in header instead
  of declaration except in base.h, and fixed other project layout
  issues).


## [Unreleased] - 2008
(kdevelop branch)
### Added
- Compress included targa files.
- Compress/decompress RLE targa files (not tested).
- Port the font rendering from C# RetroEngine (not tested).

### Changed
- Use classes and methods instead of structs and functions for nearly
  everything.

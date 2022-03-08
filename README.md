# IngameIME_Linux ![CI Status](https://github.com/Windmill-City/IngameIME_Linux/actions/workflows/CI.yaml/badge.svg)

A library for X Input Method(XIM) & Wayland Input Method(Wayland IM)

## How to use?

See [IngameIME-Common](https://github.com/Windmill-City/IngameIME-Common)

## What can you do with it?

* Activate/Deactivate Input Method

* Get available InputMethods and KeyboardLayouts in system，which are also called `InputProcessor`s

* Get the profile of the `InputProcessor`, which contains its name and the locale of the language this processor for

* Get and Set active `InputProcessor`

* Get `PreEdit` and `CandidateList` infomation during the `Composition`

## Reference

### X Input Method

* <https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#Locales_and_Internationalized_Text_Functions>

### Wayland Input Method

* <https://wayland.app/protocols/text-input-unstable-v3>

## Limitations

### XIM & Wayland IM

* Can not fetch CandidateList
* Global::getInputProcessors just return the active inputprocessor
* Can not fetch the localized name of the inputprocessor
* Cab not fetch the localized name of the locale
* InputProcessor::setActivated() has no effect
* No inputprocessor change notify callback nor input modes

### XIM (deprecated on XWayland Environment)

* No Commit notify callback
* No PreEdit & PreEditRect callback (Only tested on XWayland Environvent)

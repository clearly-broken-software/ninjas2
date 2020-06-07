# Ninjas 2
####  *a 'Clearly Broken :tm:'  Sample Slicer*
Rewrite of Ninjas, a sample slicer audio plugin.
This plugin uses the [DPF](https://github.com/DISTRHO/DPF) for the creation of audio plugins in [lv2](http://lv2plug.in/) and vst2 formats. 
A stand alone [jack](http://jackaudio.org/) app is also compiled

![Screenshot of Ninjas2](https://raw.githubusercontent.com/rghvdberg/ninjas2/master/plugins/Ninjas2/Artwork/Ninjas2.png "Ninjas2 Screenshot")


#### Goal: 
Easy to use sample slicer, quick slicing of sample and auto-mapping slices to midi note numbers.

#### Intended usage:
Primarily targeted at chopping up loops or short ( ≈ 10 - 20 seconds) samples. Think drum loops, vocal chops etc.
Currently there's no limit on imported sample length.
User can play the slices using midi notes and change the pitch with midi pitchbend.

#### Downloads:
Linux, Windows and Mac binaries for several architectures are available [here](https://github.com/rghvdberg/ninjas2/releases). There are no installers, just unzip and copy the plugin to an appropiate location.

#### New Features
+ redesigned interface
  + controls are grouped in Global, Slicing and Slice
  + the Slice box shows the currently selected slice number 
+ keyboard
  + click on key to play slice
  + red dot on key indicates which slice is currently selected in the waveform display
  + keys that don't have a slice mapped to them are greyed out

#### Known Bugs and limitations
+ some host don't work very well with the lv2 version
  + zrythm and qtractor had trouble with the lv2 version but worked fine with the vst
  + ardour, carla and muse3 worked well with the lv2
+ care should be taken when automating the playmodes and adsr
  + the automation is sent to the currently played note (slice), when multiple slices are played this leads to "undefinied behaviour"



#### Features:
+ #### Implemented:
  + load samples in audio formats as provided by [libsndfile](http://www.mega-nerd.com/libsndfile/)
  + load mp3
  + Ninjas2 is 128 voice polyphonic
  + highlight slice when midi note is received
  + maximum of 128 slices
     * slices 0 to 67 are mapped to midi note 60 to 127
     * slices 68 to 127 are mapped to midi note 0 to 58
  + slices can me snapped to onsets as detected by [aubio](https://github.com/aubio/aubio)
  + each slice has ADSR
  + each slice has 4 playmodes : 
     + one shot forward
     + one shot reverse
     + loop forward
     + loop reverse
  + pitching of samples (+/- 12 semitones) via midi pitchbend
  + user editable slice start and end points
    * **note**: slices can not overlap
    * dragging the top marker edits both start and end point, keeping the slices continuous
    * dragging the bottom markers edits the start or end point of a slice.
  + programs : store and retrieve up to 16 configurations of slices
    * normal click restores state
    * shift click copies current state to new location
  + play marker in waveform display
  + add/remove slices in waveform display : double click within 10 px of slice start/end to add or remove slice
+ #### To be implemented:
  + exponential curves in ADSR
  + adding a bar:beat:ticks ruler for easier lining up slice points
  + control for setting per slice pitch offset
+ #### Possibly implemented
  + timestretch and pitch samples using [rubberband](https://github.com/breakfastquay/rubberband)
 
  
### Build instructions

**Dependencies**

+  libgl1-mesa-dev 
+  libx11-dev
+  libsndfile1-dev
+  libsamplerate0-dev

If you want a standalone jack binary install either `libjack-dev` or `libjack-jackd2-dev`

```
git clone --recursive https://github.com/rghvdberg/ninjas2.git
cd ninjas2
make
```
There is no ~`make install`~, binaries will appear in `/path/to/ninjas2/bin/`, just copy the binaries to an appropiate location, typically ~/.vst or /usr/lib/vst on linux.

***
[![Chat on Matrix](https://matrix.to/img/matrix-badge.svg)](https://riot.im/app/#/room/#ninjas:matrix.org?action=chat)
[![Build Status](https://travis-ci.org/rghvdberg/ninjas2.svg?branch=master)](https://travis-ci.org/rghvdberg/ninjas2)

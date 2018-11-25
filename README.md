# Ninjas 2
####  *a 'Clearly Broken :tm:'  Sample Slicer*
Rewrite of Ninjas, a sample slicer audio plugin.
This plugin uses the [DPF](https://github.com/DISTRHO/DPF) for the creation of audio plugins in [lv2](http://lv2plug.in/), vst2 and dssi formats. 
A stand alone [jack](http://jackaudio.org/) app is also compiled

#### Goal: 
Easy to use sample slicer, quick slicing of sample and mapping slices to midi note numbers.
The target platform is Linux.

#### Intended usage:
Primarily targeted at chopping up loops or short ( ≈ 10 - 20 seconds) samples. Think drum loops, vocal chops etc.
Currently there's no limit on imported sample lenght.
User can play the slices using midi notes and change the pitch with midi pitchbend.

#### Features:
+ #### Implemented:
  + load samples in audio formats as provided by sndfile
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
    * dragging the top marker edits both start and end point, keeping the slices continious
    * dragging the bottom markers edits the the start or end point of a slice.
+ #### To be implemented:
  + mp3 import
  + exponential curves in ADSR
  + highlight slice when midi note is received
  + adding a bar:beat:ticks ruler for easier lining up slice points
  + states : store and retrieve up to 16 configurations of slices
+ #### Things to rework:
  + adding and removing slice points in the waveform display, currently only the control can be used for that.
    * the control should display the number of slices.
+ #### Possible features
  + timestretch and pitch samples using [bandrubberband](https://github.com/breakfastquay/rubber)
  + play marker in waveform display
  + overlay slice info on slices in waveform display
  + resizable user interface

[![Chat on Matrix](https://matrix.to/img/matrix-badge.svg)](https://riot.im/app/#/room/#ninjas:matrix.org?action=chat)

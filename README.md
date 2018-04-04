# Ninjas - Sample Slicing Plugin
#### *Ninjas Is Not Just Another Slicer*

Creating a sample slicer plugin using DPF framework.
Look at the [issues](https://github.com/rghvdberg/ninjas/issues) and feel free to report bugs, features and so on.

There's a dedicated [Matrix room (#ninjas:matrix.org)](https://riot.im/app/#/room/#ninjas:matrix.org) for Ninjas which is a good place to keep check on commits, report bugs and ask questions.

***

### Build instructions

**Dependencies**

+  libgl1-mesa-dev 
+  libx11-dev
+  libsndfile1-dev
+  libsamplerate0-dev

If you want a standalone jack binary install either `libjack-dev` or `libjack-jackd2-dev`

```
git clone --recursive https://github.com/rghvdberg/ninjas.git
cd ninjas
make
```
There is no ~`make install`~, binaries will appear in `~ninjas/bin/`, just copy the binaries to a appropiate location.

***

#### Usage
- load Ninjas in a suitable host or use the `ninjas` binary
- click Diskette drive
- load audio sample
- set number of slices using the Slices knob
- set slicing method using the slider
 **Raw**: slices up the sample in equal parts
 **Onsets**: snaps the slice start and end to the nearest detected onset
- Sample slices will be mapped to C4 (midi note 60) and up
- Change the pitch of a slice with pitchbend
- Each slice has it's own **ADSR** and **playmode** settings. Use the righthand **Slice Selector** to select the slice you want to edit

Ninjas looks like this:
![](https://github.com/rghvdberg/ninjas/blob/master/plugins/Ninjas/Artwork/screenshot.png)

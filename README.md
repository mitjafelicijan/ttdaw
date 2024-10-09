# Tiny terminal DAW

`ttdaw` is a tiny terminal based digital audio workstation made for fun,
experimentation and learning more about audio, MIDI and terminal applications
in general.

## Compile and run

```sh
make
./ttdaw -c 28:0 -s soundfonts/general-808.sf2 -p 1
```

List all available MIDI devices with `acconect -l`.

## Soundfonts

- https://dev.nando.audio/pages/soundfonts.html
- https://archive.org/details/500-soundfonts-full-gm-sets
- https://github.com/marmooo/free-soundfonts

## Developer documentation

- https://developer.apple.com/documentation/coremidi
- https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html

## Additional reading material

- https://www.izotope.com/en/learn/fundamental-frequency-harmonics.html


# Animation Data (ANIMD)

JIN's 2d animation data format

The format's changed now

```
(Number of animations)
uint32_t animations
  (Animation)
  char[8]  name
  uint32_t frames
    (Frame)
    uint32_t frame_ticks
    uint32_t x
    uint32_t y
    uint32_t w
    uint32_t h
```

There may possible be a tool later to create sprites, a spritesheet, animations, and all that in one package

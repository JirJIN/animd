# Animation Data (ANIMD)

JIN's 2d animation data format

# Binary File Spec
```
int32_t size_x
int32_t size_y

int32_t animations_num
  char[8] animation_name
  int32_t frames_num
    int32_t[size_x * size_y] id
    int32_t frame_time

```

# Text File spec

Two file sections, [SIZE] and [ANIMATIONS]

## [SIZE]
X and Y size of sprite (this enables meta sprites)

```
X: 1
Y: 1
```

Creates a 1x1 sprite.
Make sure x and y are seperated by new lines

## [ANIMATIONS]

```
Name
>1, 4
Name2
>1, 4
>2, 4
>3, 4
```

Name of the animation, ending on a new line

Every frame of the animation begins with >, it then has X * Y comma seperated values for ids.

Spritesheets should be just one long line of sprites (dimension can be configured, but it doesn't matter for the animd by default),
and id is the x position of the sprite (not coordinate, but actual sprite)

The frames beginning with > don't need to be on new lines (one frame can take multiple lines), but be sure the name
of the next animation begins on a new line (make sure every frame begins on a newline as well)

The name can only be seven characters long (it's stored as char[8] but the 8th char is '\0')

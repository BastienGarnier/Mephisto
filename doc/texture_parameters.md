OpenGL parameters : 

- GL_REPEAT: The default behavior for textures. Repeats the texture image.

- GL_MIRRORED_REPEAT:Same as GL_REPEAT but mirrors the image with each repeat.

- GL_CLAMP_TO_EDGE:Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.

- GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.

Settings for low res textures on large object

- Nearest filter : 8 bit look

- Linear interpolating filter : smooth

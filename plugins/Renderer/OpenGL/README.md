# Renderer | OpenGL plugin

> [!WARNING]
> OpenGL was deprecated in macOS 10.14.

```
if (Wayland) use EGL;
else if (supports EGL + GL) use EGL;
else fallback to GLX;
```

## Global arch
![](https://upload.wikimedia.org/wikipedia/commons/9/99/Linux_kernel_and_OpenGL_video_games.svg)
## EGL
![egl diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c2/Linux_Graphics_Stack_2013.svg/1280px-Linux_Graphics_Stack_2013.svg.png)
## NSGL
![nsgl diagram](./NSGL_diagram.png)
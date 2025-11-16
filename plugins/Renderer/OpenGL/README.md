# Renderer | OpenGL plugin

```
if (Wayland) use EGL;
else if (supports EGL + GL) use EGL;
else fallback to GLX;
```
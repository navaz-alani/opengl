# Simple OpenGL Abstraction Layer

This is a simple set of classes which aim to reduce the amount of boilerplate
and raw OpenGL calls made by the developer.

My current machine is an Intel MacBook Pro and OSx Catalina and as such, OpenGL
is available by default, though one should install `glew, glfw` and `glm` using
the `brew install <formula>` command so the header files will be available
globally.
Since OpenGL has been deprecated on MacOS, the project's `Makefile` explicitly
silences the deprecation warnings for a more peaceful compilation experience.
Additionally, this project uses version OpenGL 3.3 which is currently
implemented on my platform (not sure about older versions of OSx).

**NOTES**

* Still a lot of work left to be done, so this is not ready for use.
* Modifications to the `Makefile` need to be done to enable compilation on other
  systems (Windows maybe and Linux).

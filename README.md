I plan to write an embedded GUI myself, mainly targeting lightweight embedded projects. I estimate this will be a long-term project, and I hope to persevere.

The overall framework design is: RGB ->GDI ->GUI

That is, RGB is an independent module, GDI depends on RGB, and GUI depends on GDI

In actual development practice, projects may only need to be used for RGB or GDI. So, layering and independence are quite important. I will gradually develop and submit according to this hierarchy.

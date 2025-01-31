Game engine project. Work in progress.

What was implemented:


 1) OpenGL based rendering:

		- phong lighting model for directional light, point lights and spot lights
		- soft directional shadows using depth map, PCF sampling (9 samples)
		- soft omni-directional shadows using CUBE_MAP shadow maps and geometry shader for point/spot lights with PCF (Poisson disk).
		- qaut based camera
		- skybox
		- editor orientation mesh
		- meshes and materials with specular intensity and shininess

 2) Support models downloading. Tested formats *.obj, *gltf.
 3) Support texturing (without transparency) and materials.
 4) Engines class structure. GameEngine class contains main loop.
 5) World class is the main class that creates the app window and provides input handling and rendering methods. Will be split in the future.
 6) Player controller handles keyboard/mouse input. ['WASD' move/turn, 'mouse' camera handling, 'L' switch flashlight]
 7) Debug helpers. FPS counter.


External dependencies: these libraries should be setup at the same level as your app folder:

 - assimp downloader
 - glfw
 - glew
 - glm

Project also comtain

 - stb_image.h helper

The project uses Git LFS, so please make sure you have it installed [Git LFS version].

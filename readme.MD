# COMP3015-C1 scene project development documents

## Development environment

- **IDE:** Visual Studio 2022

## Technical implementation overview

The project uses a variety of technologies to achieve a dynamic 3D island scene, including dynamic lighting, skybox background, and texture mapping of 3D models.

## Technologies and Models

### Shader model

- **Diffuse (Key-1):** Basic diffuse reflection model, used to simulate the color of an object when a light source directly illuminates it.
- **Specular (Key-2):** Specular reflection model, used to simulate bright spots caused by light source reflection.
- **Toon Shading (Key-4):** Toon shading, which produces segmented color effects and adds a visual cartoon style.
- **Gouraud Shading (Key-5):** A smoothing algorithm for vertex shading that achieves smooth transitions by interpolating vertex lighting.
- **Flat Shading (Key-6):** Simple algorithm for patch shading, giving each patch a single color.
- **Fog (Key-6):** Fog effect, simulating the visual effects produced by depth and distance.

### Lighting model

- **Blinn-Phong Illumination Model:** is used to simulate the impact of lighting on objects, including diffuse reflection and specular reflection.
- **Spot Light (Key-3):** Spotlight lighting can simulate the direction and range of the light source.

### control

- **Mouse:** Control the viewing angle direction.
- **Keyboard:** Control movement direction and height.
- **Space:** controls animation.
### Dynamic lighting

- Dynamic lighting effects are achieved by dynamically updating light source parameters in the shader.
- Lighting includes ambient light, diffuse reflection, and specular reflection, which are calculated through shaders.

### Skybox background

- Create a skybox using a cube texture to provide an ambient background for the scene.
- The skybox is drawn at the end of the rendering process, ensuring it appears as the background.

### Texture sampling

- Texture data is loaded through the `stbi_load` function, and OpenGL functions are used to create and bind the texture.
- Texture coordinates in shaders are used to sample color values from texture images.

## Citations and technical sources

This project is based on OpenGL and standard practices in graphics programming and relies on the following libraries:

- **OpenGL**: Core graphics API for 3D graphics rendering.
- **GLFW**: Provides window management and user input processing.
- **GLEW**: Manages OpenGL extensions, allowing the use of more advanced OpenGL features.
- **GLM**: Mathematical operation library, supporting mathematical operations such as matrices and vectors.
- **Assimp**: 3D model import library, supporting model loading in multiple formats.
- **SOIL**: Simple OpenGL graphics library for loading and creating textures.
- **stb_image**: Texture loading library, supports texture creation in multiple image formats.

#pragma once

#include <glad/glad.h>

#include <string>

#include "mesh.hpp"
#include "program.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "texture.hpp"

class Application {
public:
  Application(int initial_window_width, int initial_window_height, const std::string &title)
      : window(initial_window_width, initial_window_height, title) {
    // Set callbacks
    window.set_user_pointer(this);
    window.set_key_callback(on_key);
    window.set_size_callback(on_resize);
    window.set_mouse_button_callback(on_mouse_button);
    window.set_mouse_position_callback(on_mouse_position);
  }

  void init();
  void render();

  void on_key(int key, int scancode, int actions, int mods);
  void on_mouse_position(double x, double y);
  void on_mouse_button(int button, int action, int mods);
  void on_resize(int width, int height);

  // window must be first variable declared!
  Window window;

private:
  Camera camera;
  float delta_time = 0.f;
  float last_frame = 0.f;
  GLint eye_pos_loc = -1;

  glm::mat4 projection_matrix, view_matrix, model_matrix;
  GLint view_matrix_loc = -1;
  GLint projection_matrix_loc = -1;

  ///--------------------------------------------///
  /// MAIN PROGRAM
  ///
  std::unique_ptr<ShaderProgram> program;
  GLint model_matrix_loc = -1;

  /// LIGHTS

  // Point lights
  const static int NR_POINT_LIGHTS = 2;
  glm::vec3 point_lights_positions[NR_POINT_LIGHTS] = {
      glm::vec3(-28.49f, .0f, 44.27f),
      glm::vec3(28.49f, .0f, 44.27f)
  };


  GLint point_lights_pos_loc[NR_POINT_LIGHTS];
  GLint point_lights_attenuation_constant_loc[NR_POINT_LIGHTS];
  GLint point_lights_attenuation_linear_loc[NR_POINT_LIGHTS];
  GLint point_lights_attenuation_quadratic_loc[NR_POINT_LIGHTS];
  GLint point_lights_ambient_loc[NR_POINT_LIGHTS];
  GLint point_lights_diffuse_loc[NR_POINT_LIGHTS];
  GLint point_lights_specular_loc[NR_POINT_LIGHTS];

  // Direction light
  bool dir_light_off = false;
  GLint dir_light_dir_loc = -1;
  GLint dir_light_ambient_loc = -1;
  GLint dir_light_diffuse_loc = -1;
  GLint dir_light_specular_loc = -1;

  // Spot lights
  const static int NR_SPOT_LIGHTS = 5;

  glm::vec3 spot_lights_positions[NR_SPOT_LIGHTS] = {
      glm::vec3(-19.6f, 11.5f, -8.2f),
      glm::vec3(-10.7f, 11.5f, -8.2f),
      glm::vec3(19.6f, 11.5f, -8.2f),
      glm::vec3(10.7f, 11.5f, -8.2f),
      glm::vec3(0.f, 6.54f, -10.f)
  };

  glm::vec3 spot_lights_directions[NR_SPOT_LIGHTS] = {
      glm::vec3(0.f, -11.f, -5.f),
      glm::vec3(0.f, -11.f, -5.f),
      glm::vec3(0.f, -11.f, -5.f),
      glm::vec3(0.f, -11.f, -5.f),
      glm::vec3(0.f, -7.64f, 30.f)
  };

  glm::vec3 spot_lights_colors {215 / 255.f, 237 / 255.f, 244 / 255.f};

  GLint spot_lights_pos_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_dir_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_cut_off_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_outer_cut_off_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_attenuation_constant_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_attenuation_linear_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_attenuation_quadratic_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_ambient_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_diffuse_loc[NR_SPOT_LIGHTS];
  GLint spot_lights_specular_loc[NR_SPOT_LIGHTS];


  /// MATERIALS
  GLint material_diffuse_loc = -1;
  GLint material_specular_loc = -1;
  GLint material_shininess_loc = -1;

  GLuint windows_texture = 0;

  /// OBJECTS
  // for faster loading use "objects/interior_preview.obj"
  std::vector<std::unique_ptr<Mesh>> obj = Mesh::from_file("objects/interior_preview.obj");
  std::vector<std::unique_ptr<Mesh>> windows = Mesh::from_file("objects/Windows.obj");


  ///--------------------------------------------///
  /// LIGHTS PROGRAM
  ///

  std::unique_ptr<ShaderProgram> lights_program;

  /// LIGHTS
  glm::mat4 lights_model_matrix;
  GLint lights_model_matrix_loc = -1;
  GLint lights_view_matrix_loc = -1;
  GLint lights_projection_matrix_loc = -1;

  /// MATERIALS
  GLint bulb_color_loc = -1;

  /// OBJECTS
  Mesh lights_cube = Mesh::cube();
  Mesh lights_sphere = Mesh::sphere();
  std::vector<std::unique_ptr<Mesh>> screen = Mesh::from_file("objects/screen.obj");


  ///--------------------------------------------///
  /// EXTERIOR PROGRAM
  ///
  std::unique_ptr<ShaderProgram> exterior_program;

  glm::mat4 exterior_model_matrix;
  GLint exterior_projection_matrix_loc = -1;
  GLint exterior_view_matrix_loc = -1;
  GLint exterior_model_matrix_loc = -1;

  GLint exterior_skybox_loc = -1;
  GLint exterior_eye_pos_loc = -1;

  std::vector<std::unique_ptr<Mesh>> exterior = Mesh::from_file("objects/exterior.obj");


  ///--------------------------------------------///
  /// SKYBOX PROGRAM
  std::unique_ptr<ShaderProgram> skybox_program;

  GLint skybox_projection_matrix_loc = -1;
  GLint skybox_view_matrix_loc = -1;

  /// MATERIALS
  GLint skybox_skybox_loc = -1;
  GLuint skybox_texture = 0;

  std::string faces[6] = {
      "images/cubemap/nightsky_rt.tga",
      "images/cubemap/nightsky_lt.tga",
      "images/cubemap/nightsky_up.tga",
      "images/cubemap/nightsky_dn.tga",
      "images/cubemap/nightsky_ft.tga",
      "images/cubemap/nightsky_bk.tga"
  };

  /// OBJECTS
  Mesh skybox = Mesh::cube();


  static void on_key(GLFWwindow *window, int key, int scancode, int actions, int mods) {
    Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
    this_pointer->on_key(key, scancode, actions, mods);
  }

  static void on_mouse_position(GLFWwindow *window, double x, double y) {
    Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
    this_pointer->on_mouse_position(x, y);
  }

  static void on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
    this_pointer->on_mouse_button(button, action, mods);
  }

  static void on_resize(GLFWwindow *window, int width, int height) {
    Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
    this_pointer->on_resize(width, height);
  }

  static std::string cycle_concat(int i, const char* first, const char* second);
};

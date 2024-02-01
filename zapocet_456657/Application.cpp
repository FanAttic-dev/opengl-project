#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <sstream>

using namespace std;

void Application::init() {
  // Enable depth and blend testing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  // Set clear values
  glClearColor(0.1f, 0.1f, 0.1f, .0f);
  glClearDepth(1.0);

  ///--------------------------------------------///
  /// MAIN PROGRAM

  program = make_unique<ShaderProgram>("shaders/main.vert", "shaders/main.frag");

  int position_loc = program->get_attribute_location("position");
  int normal_loc = program->get_attribute_location("normal");
  int texture_coordinate_loc = program->get_attribute_location("texture_coordinate");

  /// MATRICES
  model_matrix_loc = program->get_uniform_location("model_matrix");
  view_matrix_loc = program->get_uniform_location("view_matrix");
  projection_matrix_loc = program->get_uniform_location("projection_matrix");
  eye_pos_loc = program->get_uniform_location("eye_pos");

  /// LIGHTS
  // Pointlights
  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
      point_lights_pos_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].position"));
      point_lights_ambient_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].ambient"));
      point_lights_diffuse_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].diffuse"));
      point_lights_specular_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].specular"));
      point_lights_attenuation_constant_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].constant"));
      point_lights_attenuation_linear_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].linear"));
      point_lights_attenuation_quadratic_loc[i] = program->get_uniform_location(cycle_concat(i, "point_lights[", "].quadratic"));
  }

  // Direction light
  dir_light_dir_loc = program->get_uniform_location("dir_light.direction");
  dir_light_ambient_loc = program->get_uniform_location("dir_light.ambient");
  dir_light_diffuse_loc = program->get_uniform_location("dir_light.diffuse");
  dir_light_specular_loc = program->get_uniform_location("dir_light.specular");

  // Spot lights
  for (int i = 0; i < NR_SPOT_LIGHTS; ++i) {
      spot_lights_pos_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].position"));
      spot_lights_dir_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].direction"));
      spot_lights_cut_off_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].cut_off"));
      spot_lights_outer_cut_off_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].outer_cut_off"));
      spot_lights_attenuation_constant_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].constant"));
      spot_lights_attenuation_linear_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].linear"));
      spot_lights_attenuation_quadratic_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].quadratic"));
      spot_lights_ambient_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].ambient"));
      spot_lights_diffuse_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].diffuse"));
      spot_lights_specular_loc[i] = program->get_uniform_location(cycle_concat(i, "spot_lights[", "].specular"));
  }

  /// OBJECTS
  for (auto const& o : obj) {
      o->create_vao(position_loc, normal_loc, texture_coordinate_loc);
  }
  for (auto const& window : windows) {
      window->create_vao(position_loc, normal_loc, texture_coordinate_loc);
  }
  for (const auto& scr : screen) {
      scr->create_vao(position_loc, normal_loc, texture_coordinate_loc);
  }

  /// MATERIALS
  material_diffuse_loc = program->get_uniform_location("material.diffuse");
  material_specular_loc = program->get_uniform_location("material.specular");
  material_shininess_loc = program->get_uniform_location("material.shininess");

  // Windows
  windows_texture = load_texture_2d("objects/WindowsSurface_Color.png");


  ///--------------------------------------------///
  /// LIGHTS PROGRAM

  lights_program = make_unique<ShaderProgram>("shaders/lights.vert", "shaders/lights.frag");

  int lights_position_loc = lights_program->get_attribute_location("position");

  /// MATRICES
  lights_model_matrix_loc = lights_program->get_uniform_location("model_matrix");
  lights_view_matrix_loc = lights_program->get_uniform_location("view_matrix");
  lights_projection_matrix_loc = lights_program->get_uniform_location("projection_matrix");

  /// OBJECTS
  lights_sphere.create_vao(lights_position_loc);
  lights_cube.create_vao(lights_position_loc);

  /// MATERIALS
  bulb_color_loc = lights_program->get_uniform_location("bulb_color");


  ///--------------------------------------------///
  /// EXTERIOR PROGRAM
  exterior_program = make_unique<ShaderProgram>("shaders/exterior.vert", "shaders/exterior.frag");

  int exterior_position_loc = exterior_program->get_attribute_location("position");
  int exterior_normal_loc = exterior_program->get_attribute_location("normal");

  for (const auto& ext : exterior) {
      ext->create_vao(exterior_position_loc, exterior_normal_loc);
  }

  /// MATRICES
  exterior_projection_matrix_loc = exterior_program->get_uniform_location("projection_matrix");
  exterior_view_matrix_loc = exterior_program->get_uniform_location("view_matrix");
  exterior_model_matrix_loc = exterior_program->get_uniform_location("model_matrix");

  exterior_skybox_loc = exterior_program->get_uniform_location("skybox");
  exterior_eye_pos_loc = exterior_program->get_uniform_location("eye_pos");

  ///--------------------------------------------///
  /// SKYBOX PROGRAM
  skybox_program = make_unique<ShaderProgram>("shaders/skybox.vert", "shaders/skybox.frag");

  int skybox_position_loc = skybox_program->get_attribute_location("position");

  /// MATRICES
  skybox_projection_matrix_loc = skybox_program->get_uniform_location("projection_matrix");
  skybox_view_matrix_loc = skybox_program->get_uniform_location("view_matrix");

  /// TEXTURES
  skybox_skybox_loc = skybox_program->get_uniform_location("skybox");
  skybox_texture = load_texture_cubemap(faces);

  /// OBJECTS
  skybox.create_vao(skybox_position_loc);
}

void Application::render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float app_time_s = float(glfwGetTime());
    // calculate the new delta time
    float current_frame = app_time_s;
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    // Animated colors
    std::vector<glm::vec3> point_lights_colors = {
        glm::vec3(184 / 255.f * sin(app_time_s), 57 / 255.f, 153 / 255.f),
        glm::vec3(96 / 255.f, 159 / 255.f * cos(app_time_s), 201 / 255.f)
    };

    projection_matrix = glm::perspective(glm::radians(camera.get_zoom()), float(window.get_width()) / float(window.get_height()), 0.1f, 500.0f);
    view_matrix = camera.get_view_matrix();

    ///--------------------------------------------///
    /// LIGHTS PROGRAM

    lights_program->use();

    glUniformMatrix4fv(lights_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(lights_view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        lights_sphere.bind_vao();

        glUniform3fv(bulb_color_loc, 1, glm::value_ptr(point_lights_colors[i] * 2.f));

        model_matrix = glm::translate(glm::mat4(), point_lights_positions[i]);
        model_matrix = glm::scale(model_matrix, glm::vec3(4.f));
        glUniformMatrix4fv(lights_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        lights_sphere.draw();
    }

    // Spot lights
    for (int i = 0; i < NR_SPOT_LIGHTS-1; ++i) {
        lights_cube.bind_vao();

        glUniform3fv(bulb_color_loc, 1, glm::value_ptr(spot_lights_colors));

        model_matrix = glm::translate(glm::mat4(), spot_lights_positions[i]);
        model_matrix = glm::scale(model_matrix, glm::vec3(.2f));
        glUniformMatrix4fv(lights_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        lights_cube.draw();
    }

    ///--------------------------------------------///
    /// MAIN PROGRAM
    program->use();

    /// MATRICES
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniform3fv(eye_pos_loc, 1, glm::value_ptr(camera.get_position()));

    /// LIGHTS
    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        glUniform3fv(point_lights_pos_loc[i], 1, glm::value_ptr(point_lights_positions[i]));
        glUniform1f(point_lights_attenuation_constant_loc[i], 1.f);
        glUniform1f(point_lights_attenuation_linear_loc[i], 0.014f);
        glUniform1f(point_lights_attenuation_quadratic_loc[i], 0.0007f);
        glUniform3fv(point_lights_ambient_loc[i], 1, glm::value_ptr(point_lights_colors[i] * 0.2f));
        glUniform3fv(point_lights_diffuse_loc[i], 1, glm::value_ptr(point_lights_colors[i]));
        glUniform3fv(point_lights_specular_loc[i], 1, glm::value_ptr(point_lights_colors[i]));
    }

    // Direction light
    glUniform3f(dir_light_dir_loc, -50.f, -52.f, -316.f);
    glm::vec3 dir_light_color = {44 / 255.f, 104 / 255.f, 195 / 255.f};
    if (dir_light_off) { dir_light_color *= 0.f; };
    glUniform3fv(dir_light_ambient_loc, 1, glm::value_ptr(dir_light_color * 0.2f));
    glUniform3fv(dir_light_diffuse_loc, 1, glm::value_ptr(dir_light_color * 0.9f));
    glUniform3fv(dir_light_specular_loc, 1, glm::value_ptr(dir_light_color * 0.8f));

    // Spot lights
    for (int i = 0; i < NR_SPOT_LIGHTS; ++i) {
        glUniform3fv(spot_lights_pos_loc[i], 1, glm::value_ptr(spot_lights_positions[i]));
        glUniform3fv(spot_lights_dir_loc[i], 1, glm::value_ptr(spot_lights_directions[i]));
        if (i == 4) {
            glUniform1f(spot_lights_cut_off_loc[i], glm::cos(glm::radians(20.f)));
            glUniform1f(spot_lights_outer_cut_off_loc[i], glm::cos(glm::radians(80.f)));
            glUniform1f(spot_lights_attenuation_constant_loc[i], 1.f);
            glUniform1f(spot_lights_attenuation_linear_loc[i], 0.022f);
            glUniform1f(spot_lights_attenuation_quadratic_loc[i], 0.0019f);

            glm::vec3 screen_color {164 / 255.f, 80 / 255.f, 1 / 255.f};
            glUniform3fv(spot_lights_ambient_loc[i], 1, glm::value_ptr(screen_color * 0.2f));
            glUniform3fv(spot_lights_diffuse_loc[i], 1, glm::value_ptr(screen_color *
                                                                       sin(app_time_s * 4) +
                                                                       screen_color * 0.5f));
            glUniform3fv(spot_lights_specular_loc[i], 1, glm::value_ptr(screen_color * 0.8f));
        } else {
            glUniform1f(spot_lights_cut_off_loc[i], glm::cos(glm::radians(10.5f)));
            glUniform1f(spot_lights_outer_cut_off_loc[i], glm::cos(glm::radians(30.5f)));
            glUniform1f(spot_lights_attenuation_constant_loc[i], 1.f);
            glUniform1f(spot_lights_attenuation_linear_loc[i], 0.09f);
            glUniform1f(spot_lights_attenuation_quadratic_loc[i], 0.032f);
            glUniform3fv(spot_lights_ambient_loc[i], 1, glm::value_ptr(spot_lights_colors * 0.2f));
            glUniform3fv(spot_lights_diffuse_loc[i], 1, glm::value_ptr(spot_lights_colors * 0.5f));
            glUniform3fv(spot_lights_specular_loc[i], 1, glm::value_ptr(spot_lights_colors * 0.8f));
        }
    }

    /// OBJECTS
    for (const auto& o : obj) {
        o->bind_vao();

        /// Material properties
        // Diffuse
        glUniform1i(material_diffuse_loc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, o->get_texture_id());

        // Specular
        // no specular map
        glUniform1f(material_shininess_loc, 1.f);

        /// Model
        model_matrix = glm::mat4(1.f);
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        o->draw();
    }

    glBlendEquation(GL_FUNC_ADD);
    glBlendColor(1.f, 1.f, 1.f, 1.f);
    // Screen
    for (const auto& scr : screen) {
        scr->bind_vao();

        glBlendFunc(GL_CONSTANT_COLOR, GL_SRC_COLOR);
        glUniform1i(material_diffuse_loc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scr->get_texture_id());

        model_matrix = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

        scr->draw();
    }

    // Windows
    for (const auto& window : windows) {
        window->bind_vao();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUniform1i(material_diffuse_loc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, window->get_texture_id());

        model_matrix = glm::mat4(1.f);
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        window->draw();
    }

    ///--------------------------------------------///
    /// EXTERIOR PROGRAM
    exterior_program->use();

    glUniformMatrix4fv(exterior_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(exterior_view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    for (const auto& ext : exterior) {
        ext->bind_vao();

        glUniform1i(exterior_skybox_loc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

        exterior_model_matrix = glm::mat4();
        glUniformMatrix4fv(exterior_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(exterior_model_matrix));

        ext->draw();
    }


    ///--------------------------------------------///
    /// SKYBOX PROGRAM
    glDepthFunc(GL_LEQUAL);
    skybox_program->use();

    glUniformMatrix4fv(skybox_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    view_matrix = glm::mat4(glm::mat3(view_matrix));
    glUniformMatrix4fv(skybox_view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    skybox.bind_vao();
    glUniform1i(skybox_skybox_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
    skybox.draw();

    glDepthFunc(GL_LESS);

}

void Application::on_mouse_position(double x, double y) { camera.on_mouse_move(x, y); }

void Application::on_mouse_button(int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }

void Application::on_key(int key, int scancode, int actions, int mods) {
  switch (key) {
  case GLFW_KEY_L:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case GLFW_KEY_F:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case GLFW_KEY_1:
    dir_light_off = true;
    break;
  case GLFW_KEY_2:
    dir_light_off = false;
    break;
  default:
    camera.on_key(key, delta_time);
  }
}

void Application::on_resize(int width, int height) {
  window.resize(width, height);

  // Set the area into which we render
  glViewport(0, 0, width, height);
}

std::string Application::cycle_concat(int i, const char* first, const char* second) {
    std::stringstream ss;
    ss << first << i << second;
    return ss.str();
}

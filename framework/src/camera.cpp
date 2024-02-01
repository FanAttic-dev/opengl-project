#include "camera.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

const float Camera::min_elevation = -89.f;
const float Camera::max_elevation = 89.;
const float Camera::min_distance = 1.0f;
const float Camera::angle_sensitivity = 0.05f;
const float Camera::zoom_sensitivity = 0.1f;


Camera::Camera(const glm::vec3& position_, const float zoom_, const float speed_)
    : position(position_),
      zoom(zoom_),
      speed(speed_),
      angle_direction(-90.0f),
      angle_elevation(0.0f),
      last_x(0), last_y(0),
      is_rotating(false),
      is_zooming(false) {
  update_camera_vectors();
}

Camera::Camera()
    : position(glm::vec3(0.f, 5.f, 40.f)),
      zoom(45.f),
      speed(40.f),
      angle_direction(-90.0f),
      angle_elevation(0.0f),
      last_x(0), last_y(0),
      is_rotating(false),
      is_zooming(false) {
  update_camera_vectors();
}

void Camera::update_camera_vectors() {
  glm::vec3 nFront;
  nFront.x = cosf(glm::radians(angle_elevation)) * cosf(glm::radians(angle_direction));
  nFront.y = sinf(glm::radians(angle_elevation));
  nFront.z = cosf(glm::radians(angle_elevation)) * sinf(glm::radians(angle_direction));
  front = glm::normalize(nFront);
  right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::on_mouse_button(int button, int action, int mods) {
  // Left mouse button affects the angles
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      is_rotating = true;
    } else {
      is_rotating = false;
    }
  }
  // Right mouse button affects the zoom
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      is_zooming = true;
    } else {
      is_zooming = false;
    }
  }
}

void Camera::on_mouse_move(double x, double y) {
  float dx = float(x - last_x);
  float dy = float(y - last_y);
  last_x = static_cast<int>(x);
  last_y = static_cast<int>(y);

  if (is_rotating) {
    angle_direction += dx * angle_sensitivity;
    angle_elevation -= dy * angle_sensitivity;

    // Clamp the results
    if (angle_elevation > max_elevation)
      angle_elevation = max_elevation;
    if (angle_elevation < min_elevation)
      angle_elevation = min_elevation;
  }

  if (is_zooming) {
      if (zoom >= 1.f && zoom <= 45.f) {
          zoom += dy * zoom_sensitivity;
      }
      if (zoom <= 1.f) {
          zoom = 1.f;
      }
      if (zoom >= 45.f) {
          zoom = 45.f;
      }
  }

  update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::get_position() const {
    return position;
}

float Camera::get_zoom() const {
    return zoom;
}

void Camera::on_key(int key, float delta_time) {
    float velocity = speed * delta_time;

    switch (key) {
    case GLFW_KEY_W:
        position += front * velocity;
        break;
    case GLFW_KEY_S:
        position -= front * velocity;
        break;
    case GLFW_KEY_A:
        position -= right * velocity;
        break;
    case GLFW_KEY_D:
        position += right * velocity;
        break;
    case GLFW_KEY_DOWN:
        position -= up * velocity;
        break;
    case GLFW_KEY_UP:
        position += up * velocity;
        break;
    }

}

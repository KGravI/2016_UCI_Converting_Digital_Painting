#version 120

uniform mat4 mvp_matrix;
uniform mat4 mv_matrix;
uniform mat3 normal_matrix;

uniform vec3 mouse_points;

uniform float mouse_click;
uniform vec4 vp;


attribute vec4 a_vertex;
attribute vec3 a_normal;
attribute vec3 a_color;

varying vec3 v_vertex;
varying vec3 v_normal;
varying float v_color_change;
varying vec3 v_color;


void main() {
  v_normal = normalize (normal_matrix * a_normal);
  v_vertex = (mv_matrix * a_vertex).xyz;

  v_color_change=0;
  v_color=a_color;

  gl_Position = mvp_matrix * a_vertex;


}
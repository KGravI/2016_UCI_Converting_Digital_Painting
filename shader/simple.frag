uniform vec3 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec3 v_color;

float x = gl_FragCoord.x/640;
float y = gl_FragCoord.y/640;
float z = gl_FragCoord.z/gl_FragCoord.a; 
uniform vec3 mouse_points;
uniform float mouse_click;
uniform float brush_size;


float distanceOfMouse(float mouseX, float mouseY, float x, float y)
{
	return sqrt((mouseX - x)*(mouseX - x) + (mouseY - y)*(mouseY - y));
}

vec4 directional_light() {
  vec4 color = vec4(0);
  vec3 normal = normalize(v_normal);
  vec3 light_vector= normalize(light_position);
  vec3 reflect_vector = reflect(-light_vector, normal);
  vec3 view_vector = normalize(-v_vertex);
  float plus_color=0.0f;

 if(distanceOfMouse(mouse_points.x, mouse_points.y,x*2.0-1.0,y*2.0-1.0)<=brush_size && mouse_click >1.0f ) plus_color=10.0f;

  color += (light_ambient* material_ambient);
  float ndotl = max(0.0, dot(normal, light_vector) );
  color += (ndotl * light_diffuse*vec4(material_diffuse.r+plus_color , material_diffuse.g, material_diffuse.b+v_color.b, 1.0f));

  float rdotv = max(0.0, dot(reflect_vector, view_vector) );
  color += (pow(rdotv, material_shininess) * light_specular* material_specular);

  return color;
}
void main() {
  gl_FragColor= directional_light();
}


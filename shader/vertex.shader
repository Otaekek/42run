#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_uv;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 pos_color;
out vec2 uv;

void main(void)
{
/*	vec4 position;

	pos_color = in_Position;
	position = vec4(in_Position.xyz, 1.0);
	pos_color = (position * M).xyz;
	position = position * M;
	position.w = 1;
	position *= V;
	position.w = 1;
	position *= P;
	pos_color = (position * M).xyz;
	position.w = 1;
	gl_Position = position;
*/
//pos_color = position.xyz;


	uv = in_uv;
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
    gl_Position *= M;
    gl_Position *= V;
    gl_Position *= P;
	pos_color = gl_Position.xyz;
}
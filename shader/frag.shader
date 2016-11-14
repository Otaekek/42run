#version 400

in vec3 pos_color;
in vec2 uv;

uniform sampler2D text;
uniform float alpha;

out vec4 FragColor;


void main(void)
{
	vec2 t;
	float far;
	vec4 red;

	red = vec4(1,0,0,0);
	far = pos_color.z;
	far /= 2;
	far += 0.5f;
	far = 8.0f - far;
	far /= 9;
	t.x = 0.4;
	t.y = 0.3;
	t = uv;
	float a;
	FragColor = vec4(0, 0,0, 1);

	if (texture(text, t).r + texture(text, t).g + texture(text, t).b == 0)
	   a = 0;
	  else
	   a = alpha *2;
	FragColor = vec4(texture(text, t).r * far, texture(text, t).g * far, texture(text, t).b * far, a);
	if (alpha == 0.3)
	   FragColor += red;
}
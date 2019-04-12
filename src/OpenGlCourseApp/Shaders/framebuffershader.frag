#version 430

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D theTextureDiffuse;

const float offset = 1.0 / 300.0;

// create new uniform in the shader class for the screenspace texture

void main() {

	
	vec2 offsets[9] = vec2[](
	     vec2(-offset , offset),
	     vec2( 0.0f   , offset),
	     vec2( offset , offset),
	     vec2(-offset , 0.0f  ),
	     vec2( 0.0f   , 0.0f  ),
	     vec2( offset , 0.0f  ),
	     vec2(-offset ,-offset),
	     vec2( 0.0f   ,-offset),
	     vec2( offset ,-offset)
	);

	float kernel[9] = float[]( 
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
		2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(theTextureDiffuse, TexCoords.st + offsets[i]));
	}

	vec3 col = vec3(0.0f);
	for (int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}

	vec4 brightnessf = vec4(texture(theTextureDiffuse, TexCoords).xyz, 1.0f);
	float brightness = (brightnessf.x + brightnessf.y + brightnessf.z) / 3.0f;
	//FragColor = ( (vec4(col, 1.0f) * texture(theTextureDiffuse, TexCoords) ) - (1.0f - brightness)) + texture(theTextureDiffuse,TexCoords); 
	
	
	FragColor = texture(theTextureDiffuse, TexCoords);
}
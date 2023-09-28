#version 410

layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec2 iv2tex_coord;
layout(location = 2) in vec3 iv3normal;
layout(location = 3) in vec3 iv3tangent;
layout(location = 4) in vec3 iv3bitangent;

uniform mat3 um3n;
uniform mat4 um4m;
uniform mat4 um4v;
uniform mat4 um4p;
uniform mat4 lightSpaceMatrix;

//environment map
uniform bool useEnvMap;

out VertexData
{
    //vec3 N; // eye space normal
    //vec3 L;  // eye space light vector
    //vec3 H; // eye space halfway vector
    
	vec3 vNormal;
	vec3 vPos;
    vec2 texcoord;
	vec4 FragPosLightSpace;

	mat3 TBN;
} vertexData;

void main()
{
		vertexData.vPos = vec3(um4m * vec4(iv3vertex, 1.0));
		vertexData.texcoord = iv2tex_coord;
		if(useEnvMap) vertexData.vNormal = mat3(transpose(inverse(um4m))) * iv3normal; 
		else {
			mat3 modelVector = transpose(inverse(mat3(um4m)));
			vec3 T = normalize(modelVector * iv3tangent);
			vec3 B = normalize(modelVector * iv3bitangent);
			vec3 N = normalize(modelVector * iv3normal);
			vertexData.TBN = mat3(T, B, N);

			//vertexData.vNormal = normalize(modelVector * iv3normal);
			vertexData.vNormal = transpose(inverse(mat3(um4m))) * iv3normal;
			vertexData.FragPosLightSpace = lightSpaceMatrix  * vec4(vertexData.vPos, 1.0);
		}
		gl_Position = um4p * um4v * um4m * vec4(iv3vertex, 1.0);
}
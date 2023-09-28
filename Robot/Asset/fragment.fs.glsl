#version 410
layout(location = 0) out vec4 fragColor;

in VertexData
{
    //vec3 N; // eye space normal
    // vec3 L;  // eye space light vector
    //vec3 H; // eye space halfway vector

	vec3 vNormal;
	vec3 vPos;
    vec2 texcoord;
	vec4 FragPosLightSpace;

	mat3 TBN;
} vertexData;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform samplerCube cubeMap;
uniform sampler2D depthMap;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform bool normalMapping;

//environment map
uniform bool useEnvMap;
uniform bool reflectOrRefract;

//shader material
uniform int shaderMaterialID;  //0 = None, 1 = White noise, 2 = Rusty, 3 = Alien

float rand(vec2 co) {
	co = mod(co, 10000.0);
	return fract(sin(dot(co, glm::vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 rand2(vec2 co){
	co = mod(co, 10000.0);
	return fract(sin(vec2(dot(co, vec2(127.1, 311.7)), dot(co, vec2(269.5, 183.3)))) * 4);
}

float value_noise(vec2 coord){
	vec2 i = floor(coord);
	vec2 f = fract(coord);

	float tl = rand(i), tr = rand(i + vec2(1.0, 0.0));
	float bl = rand(i + vec2(0.0, 1.0)), br = rand(i + vec2(1.0, 1.0));

	vec2 cubic = f * f * (3.0-2.0*f);
	float topmix = mix(tl, tr, cubic.x);
	float botmix = mix(bl, br, cubic.x);
	float wholemix = mix(topmix, botmix, cubic.y);

	return wholemix;
}

float perlin_noise(vec2 coord){
	vec2 i = floor(coord);
	vec2 f = fract(coord);

	float tl = rand(i) * 6.283, tr = rand(i + vec2(1.0, 0.0)) * 6.283;
	float bl = rand(i + vec2(0.0, 1.0)) * 6.283, br = rand(i + vec2(1.0, 1.0)) * 6.283;

	vec2 tlvec = vec2(-sin(tl), cos(tl));
	vec2 trvec = vec2(-sin(tr), cos(tr));
	vec2 blvec = vec2(-sin(bl), cos(bl));
	vec2 brvec = vec2(-sin(br), cos(br));
	
	float tldot = dot(tlvec, f);
	float trdot = dot(trvec, f - vec2(1.0, 0.0));
	float bldot = dot(blvec, f - vec2(0.0, 1.0));
	float brdot = dot(brvec, f - vec2(1.0, 1.0));
	
	tldot = abs(tldot);
	trdot = abs(trdot);
	bldot = abs(bldot);
	brdot = abs(brdot);
	
	vec2 cubic = f * f * (3.0 - 2.0 * f);

	float topmix = mix(tl, tr, cubic.x);
	float botmix = mix(bl, br, cubic.x);
	float wholemix = mix(topmix, botmix, cubic.y);

	return wholemix;
}

float cellular_noise(vec2 coord){
	vec2 i = floor(coord);
	vec2 f = fract(coord);

	float min_dist = 99999.0;
	for(float x = -1.0; x <= 1.0; x++){
		for(float y = -1.0; y<= 1.0;y++){
			vec2 node = rand2(i + vec2(x, y) + vec2(x, y));
			float dist = sqrt((f - node).x * (f - node).x + (f - node).y * (f - node).y);
			min_dist = min(min_dist, dist);
		}
	}
	return min_dist;
}

float FBM(vec2 coord){
	int OCTAVES = 4;

	float normalize_factor = 0.0f;
	float value = 0.0, scale = 0.5;

	for(int i = 0;i<OCTAVES;i++){
		value += value_noise(coord) * scale;
		normalize_factor += scale;
		coord *= 2.0;
		scale *= 0.5;
	}
	return value / normalize_factor;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 _normal, vec3 _lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(_normal, _lightDir)), 0.005);  
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    return shadow;
}

void main()
{
	vec3 R, texColor;
	if(useEnvMap){  //environmental mapping
		if(reflectOrRefract)
		{
			R = reflect(normalize(vertexData.vPos - cameraPos), normalize(vertexData.vNormal));
		}
		else {
			R = refract(normalize(vertexData.vPos - cameraPos), normalize(vertexData.vNormal),  1.0f / 1.52f);
		}
		texColor = texture(cubeMap, R).rgb;
	}
	else{ //original texture
		vec3 normal = normalize(vertexData.vNormal);

		if(normalMapping){
			normal = texture(normalMap, vertexData.texcoord).rgb;
			normal = normal * 2.0 - 1.0;
			normal = normalize(vertexData.TBN * normal);
		}

		vec3 color = texture(diffuseMap, vertexData.texcoord).rgb; 
		if(shaderMaterialID > 0){
			if(shaderMaterialID == 1){  //White noise
				float value = rand(vertexData.texcoord * 50.0);
				color = value>0.5?vec3(1.0):vec3(0.0);
			}
			else if(shaderMaterialID == 2){  //Rusty
				float value = FBM(vertexData.texcoord * 50.0);
				color = vec3(value, value / 2.0, value / 4.0);
			}
			else if(shaderMaterialID == 3){  //Alien
				float value = perlin_noise(vertexData.texcoord * 50);
				float value2 = cellular_noise(vertexData.texcoord * 60);
				value = sin(vertexData.texcoord.x * 2.0 + vertexData.texcoord.y + value + value2);
				color = vec3(min(1-value, value2), min(1-value2, value), value);
			}
		}


		vec3 lightColor = vec3(0.3);
		//ambient
		vec3 ambient = 0.3 * color;
		//diffuse
		vec3 lightDir = normalize(lightPos - vertexData.vPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * lightColor;
		//specular
		vec3 viewDir = normalize(cameraPos - vertexData.vPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
		vec3 specular = spec * lightColor;

		float shadow = ShadowCalculation(vertexData.FragPosLightSpace, normal, lightDir);
		texColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	}
	fragColor = vec4(texColor.rgb, 1.0);
}
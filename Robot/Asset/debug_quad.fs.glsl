#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureMap;
uniform sampler2D prevTextureMap;
uniform int filter_selection;
uniform float mixRate;  //percentage of use of 
uniform float rows;
uniform float cols;
//uniform bool debug;

float dx = 1.0/1024.0, dy = 1.0/1024.0;

void main()
{   
    if(filter_selection == 0){
        FragColor = vec4(texture(textureMap, TexCoords).rgb, 1.0);
    }
    else if(filter_selection == 1){
        float nx = 1.0 / cols, ny = 1.0 / rows;
        vec2 coord;
        coord[0] = floor(TexCoords[0] * cols) / cols;
        coord[1] = floor(TexCoords[1] * rows) / rows;
        float l = coord[0] - nx / 2.0, bot = coord[1] - ny / 2.0;
        float r = l + nx, top = bot + ny;
        l = max(l, 0.0); bot = max(bot, 0.0);
        r = min(r, 1.0); top = min(top, 1.0);
        float w = 1.0 / ((r - l) / dx + 1) / ((top - bot) / dx + 1);
        vec3 texColor = vec3(0.0);
        for(float x = l;x<=r;x+=dx){
            for(float y = bot;y <= top;y+=dy){
                texColor += texture(textureMap, vec2(x,y)).rgb * w;
            }
        }
        FragColor = vec4(texColor, 1.0);
    }
    else {
        vec3 texColor1 = texture(textureMap, TexCoords).rgb;
        vec3 texColor2 = texture(prevTextureMap, TexCoords).rgb;
        vec3 texColor = ((texColor1 * (1.0 - mixRate)) + (texColor2 * mixRate));
        FragColor = vec4(texColor, 1.0);
       // if(debug)  FragColor = vec4(texColor2, 1.0);
    }
}
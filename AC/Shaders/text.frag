#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text; 
uniform vec3 textColor;  
uniform float alpha;     

void main()
{
    float mask_value = texture(text, TexCoords).r;
    
    color = vec4(textColor, 1.0) * mask_value;
    color.a *= alpha;
}
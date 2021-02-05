#version 450 core

uniform sampler2D Texture0;
uniform sampler2D Texture1;

in vec2 TexCoords;

void main()
{  
    vec4 texel0, texel1;

    texel0 = texture2D(Texture0, TexCoords);
    texel1 = texture2D(Texture1, TexCoords);

    gl_FragColor = texel1 * texel0.a;
}
#version 410

/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=8) in vec4 TexCoord;

uniform mat4 matrProj;
uniform mat4 matrVisu;

out Attribs {
    vec2 texCoord;
} AttribsOut;

void main( void )
{
    vec4 pos = matrProj * matrVisu * Vertex;
    gl_Position = pos;
    AttribsOut.texCoord = TexCoord.st;
}

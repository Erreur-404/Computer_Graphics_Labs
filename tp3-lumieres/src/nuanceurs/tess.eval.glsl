#version 410

layout(quads) in;

in Attribs {
    vec2 texCoord;
} AttribsIn[];

out Attribs {
    vec4 couleur;
} AttribsOut;

uniform sampler2D heighMapTex;

vec4 interpole(vec4 v0, vec4 v1, vec4 v2, vec4 v3)
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec4 v01 = mix(v0, v1, gl_TessCoord.x);
    vec4 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}

void main()
{
    vec4 coul = texture(heighMapTex, gl_TessCoord.xy);
    // interpoler la position et les attributs selon gl_TessCoord
    vec4 pos = interpole( gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position );
    pos.y += 10 * coul.r;
    gl_Position = pos;

    AttribsOut.couleur = coul;
}

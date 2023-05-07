#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Attribs {
    vec4 couleur;
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec3 normale;
} AttribsOut;

void main()
{
    vec3 arete1 = (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    vec3 arete2 = (gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    vec3 normale = cross(arete1, arete2);

    for(int i = 0; i < gl_in.length(); i++ ) {
        gl_Position = gl_in[i].gl_Position;
        AttribsOut.couleur = AttribsIn[i].couleur;
        AttribsOut.normale = normale;
        EmitVertex();
    }

    EndPrimitive();
}


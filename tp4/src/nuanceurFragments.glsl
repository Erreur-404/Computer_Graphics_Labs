#version 410

uniform sampler2D leLutin;
uniform int texnumero;

in Attribs {
    vec2 texCoord;
    vec4 couleur;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    FragColor = AttribsIn.couleur;

    if ( texnumero != 0 )
    {
        vec4 couleur = texture( leLutin, AttribsIn.texCoord );
        FragColor.rgb = mix(FragColor.rgb, couleur.rgb, 0.6);
        FragColor.a = couleur.a == 0 ? couleur.a : AttribsIn.couleur.a;
    }
    if (FragColor.a < 0.1) discard;
}

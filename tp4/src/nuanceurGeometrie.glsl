#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matrProj;

uniform int texnumero;

layout (std140) uniform varsUnif
{
    float tempsDeVieMax;       // temps de vie maximal (en secondes)
    float temps;               // le temps courant dans la simulation (en secondes)
    float dt;                  // intervalle entre chaque affichage (en secondes)
    float gravite;             // gravité utilisée dans le calcul de la position de la particule
    float pointsize;           // taille des points (en pixels)
};

in Attribs {
    vec4 couleur;
    float tempsDeVieRestant;
    float sens; // du vol (partie 3)
    float hauteur; // de la particule dans le repère du monde (partie 3)
} AttribsIn[];

out Attribs {
    vec2 texCoord;
    vec4 couleur;
} AttribsOut;

// la hauteur minimale en-dessous de laquelle les lutins ne tournent plus (partie 3)
const float hauteurInerte = 8.0;

void calculerTexture(float nLutin) {
    int num = int ( mod ( 18.0 * AttribsIn[0].tempsDeVieRestant, nLutin ) ); // 18 Hz
    AttribsOut.texCoord.x = ( AttribsOut.texCoord.x + num ) / nLutin ;   
}

void main()
{
    vec2 coins[4];
    coins[0] = vec2( -0.5,  0.5 );
    coins[1] = vec2( -0.5, -0.5 );
    coins[2] = vec2(  0.5,  0.5 );
    coins[3] = vec2(  0.5, -0.5 );

    for ( int i = 0 ; i < 4 ; ++i )
    {
        float fact = gl_in[0].gl_PointSize;
        gl_PointSize = gl_in[0].gl_PointSize;

        vec2 decalage = coins[i]; // on positionne successivement aux quatre coins
        vec4 pos = vec4( gl_in[0].gl_Position.xy + fact * decalage, gl_in[0].gl_Position.zw );

        gl_Position = matrProj * pos;    // on termine la transformation débutée dans le nuanceur de sommets

        vec4 tmp = AttribsIn[0].couleur;
        // On fait disparaitre graduellement les particules lorsqu'il leur reste moins de 20% de temps de vie.
        if (AttribsIn[0].tempsDeVieRestant  / tempsDeVieMax < 0.20 ) {
            // tmp.a = 1 - (tempsDeVieMax - AttribsIn[0].tempsDeVieRestant) / tempsDeVieMax;
            tmp.a = AttribsIn[0].tempsDeVieRestant / (0.20 * tempsDeVieMax);
        }
        AttribsOut.couleur = tmp;
        vec2 texCoord = coins[i] + vec2( 0.5, 0.5 ); // on utilise coins[] pour définir des coordonnées de texture
        
        if(AttribsIn[0].sens == -1.0) {
            texCoord = coins[( i + 2 ) % 4] + vec2(0.5, 0.5);
        }

        const float nlutinsOiseau = 16.0; // 16 positions de vol dans la texture
        const float nlutinsMario = 12.0; // 12 positions de vol dans la texture
        const float nlutinsMarioSMW = 20.0; // 20 positions de vol dans la textur

        AttribsOut.texCoord = texCoord;
        if(AttribsIn[0].hauteur > hauteurInerte) {
            if (texnumero == 1) { // Si on traite l'oiseau
                calculerTexture(nlutinsOiseau);
            }
            else if (texnumero == 2) { // Si on traite le flocon
                float angle = 6.0 * AttribsIn[0].tempsDeVieRestant;
                mat2 matriceRotation = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
                AttribsOut.texCoord = coins[i] * matriceRotation + vec2(0.5, 0.5);
            } 
            else if (texnumero == 3) { // Si on traite le mario
                calculerTexture(nlutinsMario);
            }
            else if (texnumero == 4) { // Si on traite le mario 2
                calculerTexture(nlutinsMarioSMW);
            }
        } else {
            if (texnumero == 1) { // Si on traite l'oiseau
                AttribsOut.texCoord.x = texCoord.x / nlutinsOiseau;
            }
            else if (texnumero == 2) { // Si on traite le flocon
                AttribsOut.texCoord = coins[i] + vec2(0.5, 0.5);
            } 
            else if (texnumero == 3) { // Si on traite le mario
                AttribsOut.texCoord.x = texCoord.x / nlutinsMario;
            }
            else if (texnumero == 4) { // Si on traite le mario2
                AttribsOut.texCoord.x = texCoord.x / nlutinsMarioSMW;
            }
        }
        EmitVertex();
    }
}

#version 410

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient[3];
    vec4 diffuse[3];
    vec4 specular[3];
    vec4 position[3];      // dans le repère du monde
    vec3 spotDirection[3]; // dans le repère du monde
    float spotExponent;
    float spotAngleOuverture; // ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource;

// Définition des paramètres des matériaux
layout (std140) uniform MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} FrontMaterial;

// Définition des paramètres globaux du modèle de lumière
layout (std140) uniform LightModelParameters
{
    vec4 ambient;       // couleur ambiante globale
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

layout (std140) uniform varsUnif
{
    // partie 1: illumination
    int typeIllumination;     // 0:Gouraud, 1:Phong
    bool utiliseSpot;         // indique si on utilise des lumière de type spot ou point
    bool utiliseBlinn;        // indique si on veut utiliser modèle spéculaire de Blinn ou Phong
    bool utiliseDirect;       // indique si on utilise un spot style Direct3D ou OpenGL
    bool afficheNormales;     // indique si on utilise les normales comme couleurs (utile pour le débogage)
    // partie 2: texture
    float tempsGlissement;    // temps de glissement
    int iTexCoul;             // numéro de la texture de couleurs appliquée
    // partie 3b: texture
    int iTexNorm;             // numéro de la texture de normales appliquée
};

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=8) in vec4 TexCoord;

out Attribs {
    vec3 lumiDir[3];
    vec3 normale, obsVec;
    vec2 textCoord;
    vec3 spotDir[3];
} AttribsOut;
 
void main(void)
{
    // appliquer la transformation standard du sommet (P * V * M * sommet)
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // calculer la normale (N) qui sera interpolée pour le nuanceur de fragments
    AttribsOut.normale = matrNormale * Normal;

    // calculer la position (P) du sommet (dans le repère de la caméra)
    vec3 pos = vec3( matrVisu * matrModel * Vertex );

    // calculer le vecteur de la direction (L) de la lumière (dans le repère de la caméra)
    for(int i = 0; i < 3; i++) {
        AttribsOut.spotDir[i] = mat3(matrVisu) * -LightSource.spotDirection[i];
        AttribsOut.lumiDir[i] = ( matrVisu * LightSource.position[i] ).xyz - pos;
    }

    // calculer le vecteur de la direction (O) vers l'observateur (dans le repère de la caméra)
    AttribsOut.obsVec = ( -pos );
    // Code inspire de l"equipe de Majeed Abdul Baki et Victor Gilbert
    AttribsOut.textCoord = TexCoord.xy;
}

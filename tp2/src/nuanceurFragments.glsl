#version 410

////////////////////////////////////////////////////////////////////////////////

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;      // dans le repère du monde
    vec3 spotDirection; // dans le repère du monde
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
    vec4 ambient;       // couleur ambiante
    bool localViewer;   // observateur local ou à l'infini?
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

////////////////////////////////////////////////////////////////////////////////

uniform int illumination; // on veut calculer l'illumination ?
uniform int monochromacite; // on appliquer la monochromacite ?

const bool utiliseBlinn = true;

in Attribs {
    vec4 couleur;
    vec3 normale;
} AttribsIn;

out vec4 FragColor;

vec4 calculerReflexion( in vec3 L, in vec3 N, in vec3 O )
{
    float attenuation = 0.5;
    vec4 coul = vec4(0);

    // calculer la composante ambiante pour la source de lumière
    coul += FrontMaterial.ambient * LightSource.ambient;

    // calculer l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calculer la composante diffuse
        coul +=  attenuation * FrontMaterial.diffuse * LightSource.diffuse * NdotL;

        // calculer la composante spéculaire (Blinn ou Phong : spec = BdotN ou RdotO )
        float spec = ( utiliseBlinn ?
                       dot( normalize( L + O ), N ) : // dot( B, N )
                       dot( reflect( -L, N ), O ) ); // dot( R, O )
        if ( spec > 0 ) coul += attenuation * FrontMaterial.specular * LightSource.specular * pow( spec, FrontMaterial.shininess );
    }

    return( coul );
}


void main( void )
{
    // la couleur du fragment est la couleur interpolée
    vec4 tempFragColor = AttribsIn.couleur;
    if(bool(illumination)) {
        vec3 O = normalize(vec3( 0, 0, 1 ));
        vec3 L = normalize(vec3( 0, 0, 1 ));
        vec3 N = normalize(gl_FrontFacing ? AttribsIn.normale : -AttribsIn.normale);
        vec4 coul = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;

        coul += calculerReflexion(L, N, O);
        coul += AttribsIn.couleur;
        tempFragColor = clamp(coul, 0.0, 1.0);
    }

    if(bool(monochromacite)) {
        float Y = tempFragColor.x * 0.229 + tempFragColor.y * 0.587 + tempFragColor.z * 0.114;
        tempFragColor.x = Y;
        tempFragColor.y = Y;
        tempFragColor.z = Y;
    }
    FragColor = tempFragColor;
}

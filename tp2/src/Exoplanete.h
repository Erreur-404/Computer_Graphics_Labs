#ifndef __EXOPLANETE_H__
#define __EXOPLANETE_H__

// affiche la position courante du repère (pour débogage)
void afficherRepereCourant( int num = 0 )
{
    // obtenir le prog courant
    GLint prog = 0;
    glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
    // utiliser le prog de base
    glUseProgram( progBase );
    glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    FenetreTP::afficherAxes( 1.5, 3.0 );
    // remettre le prog courant
    glUseProgram( prog );
}

//
// Un Isocaedre(exoplanete)
//
class Exoplanete
{
    static FormeSphere *sphere; // une sphère centrée de rayon 1 pour les satellites
public:
    Exoplanete( float r = 1.0, float h = 0.0, float ang = 0.0, float vit = 1.0, float tai = 0.5,
             glm::vec3 coulCorps = glm::vec3( 1.0, 0.2, 0.0 ) )  // rouge légèrement orangé
        : vao(0), vboExoplaneteSommets(0), vboExoplaneteConnec(0), couleurPlanete(coulCorps), rayon(r), hauteur(h), angle(ang), vitesse(vit), taille(tai)
    {
        // créer une planete graphique
        initialiserGraphique();
    }
    ~Exoplanete()
    {
        conclureGraphique();
    }
    void initialiserGraphique()
    {
        // créer quelques formes
        if ( sphere == NULL ) sphere = new FormeSphere( 1.0, 8, 8, true );

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la planete
        glBindVertexArray( vao );

        // créer le VBO pour les sommets
        glGenBuffers( 1, &vboExoplaneteSommets );
        glBindBuffer( GL_ARRAY_BUFFER, vboExoplaneteSommets );
        glBufferData( GL_ARRAY_BUFFER, sizeof(gExoplaneteSommets), gExoplaneteSommets, GL_STATIC_DRAW );
        // faire le lien avec l'attribut du nuanceur de sommets
        glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
        glEnableVertexAttribArray(locVertex);

        // créer le VBO la connectivité
        glGenBuffers( 1, &vboExoplaneteConnec );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboExoplaneteConnec );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(gExoplaneteConnec), gExoplaneteConnec, GL_STATIC_DRAW );

        glBindVertexArray(0);
    }
    void conclureGraphique()
    {
        delete sphere;
    }

    // obtenir la matrice de modélisation qui représente la position courante de cette planete
    MatricePipeline obtenirMatriceCourante( )
    {
        MatricePipeline mtc;
        mtc.LoadIdentity();
        // amener le repère à la position courante de cette planete
        mtc.Rotate( angle, 0, 1, 0 );
        mtc.Translate( rayon, hauteur, 0.0 );
        mtc.Scale( taille, taille, taille );

        if (Etat::orbiteGeosynchrone) {
            mtc.Translate( 5.5 , 1.0, 0.0 );
            mtc.Rotate( 90, 0.0, 1.0, 0.0 );
        } else {
            mtc.Rotate( angle*4, 0.0, 1.0, 0.0 );
            mtc.Translate( 3.5, 0.0, 0.0 );
            mtc.Scale( 0.3, 0.3, 0.3 );

            mtc.Translate( 5.5 , 3.0, 0.0 );
            mtc.Rotate( 90, 0.0, 1.0, 0.0 );
        }
        //mtc = mtc.PushMatrix();

        // Utiliser ou non la rotation geosynchrone
        
        // std::cout << "//@Exoplanete, obtenirMatriceCourante;" << " determinant=" << glm::determinant( glm::mat4(m) ) << std::endl;
        return mtc;
    }

    void afficher()
    {
        matrModel.PushMatrix(); { // sauvegarder la transformation courante

            // amener le repère à la position courante de cette planete
            matrModel.Rotate( angle, 0, 1, 0 ); // tourner selon l'angle
            matrModel.Translate( rayon, hauteur, 0.0 ); // se déplacer dans la direction de l'axe des X ainsi tourné
            
            // afficher la planète
            glVertexAttrib3fv( locColor, glm::value_ptr(couleurPlanete) );
            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            // ajuster selon la taille
            matrModel.Scale( taille, taille, taille );

            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
            glBindVertexArray( vao );
            glDrawElements( GL_TRIANGLES, sizeof(gExoplaneteConnec)/sizeof(GLuint), GL_UNSIGNED_INT, 0 );
            glBindVertexArray(0);

            // donner la couleur du satellite
            glVertexAttrib3f( locColor, 0.75, 0.75, 0.75); // gris

            // afficher le satellite
            matrModel.Rotate( angle*4, 0.0, 1.0, 0.0 );
            matrModel.Translate( 3.5, 0.0, 0.0 );
            matrModel.Scale( 0.3, 0.3, 0.3 );
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            sphere->afficher();
            
            }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void avancerPhysique()
    {
        angle += Etat::sens * Etat::dt * vitesse;
    }

    // les variables de la planete
    GLuint vao = 0;
    GLuint vboExoplaneteSommets = 0;
    GLuint vboExoplaneteConnec = 0;
    glm::vec3 couleurPlanete; // la couleur
    float rayon;            // en unités
    float hauteur;          // en unités
    float angle;            // en degrés
    float vitesse;          // en degrés/seconde
    float taille;           // en unités
};

FormeSphere* Exoplanete::sphere = NULL;

#endif
#ifndef __TROUDEVER_H__
#define __TROUDEVER_H__

#include <vector>
#include <iterator>
#include "Exoplanete.h"

//
// Le trou de ver
//
class TrouDeVer
{
    static FormeCube* cubeFil;
    static FormeQuad* quad;
    static FormeSphere* soleil;
    // Partie 1 2 et 3 ... Il manque quelque chose ici pour que ça compile ... indice tout en bas de ce fichier
    // ...
public:
    TrouDeVer()
        : locillumination(-1), locmonochromacite(-1)
    {
        // les hauteurs variées des planetes
        float hauteur[] = { -11.3, -6.0, 4.5, 3.0, -2.0, -8.0, -5.0, -3.0, 8.6, 7.0, -1.0, 1.0, 5.1, 6.3, 12.0, -4.0, -9.0, -7.0 };

        // créer un trou de ver graphique
        glUseProgram(prog);
        initialiserGraphique();

        // initialiser la génération de valeurs aléatoires pour la création de planetes
        srand(time(NULL));

        // afficher les planètes
        for (unsigned int i = 0; i < sizeof(hauteur) / sizeof(hauteur[0]); ++i)
        {
            // rayon (distance au centre de rotation) aléatoire
            float rayon = glm::mix(45.0, 50.0, rand() / ((double)RAND_MAX));

            // un angle aléatoire en degrés
            float angle = glm::mix(0, 360, rand() / ((double)RAND_MAX));

            // vitesse de rotation aléatoire
            float vit = glm::mix(-10.0, 10.0, rand() / ((double)RAND_MAX));
            vit += 4.0 * glm::sign(vit); // ajouter ou soustraire 4.0 selon le signe de vit afin d'avoir : 4.0 <= abs(vit) <= 12.0

            // taille aléatoire
            float taille = glm::mix(1.5, 2.0, rand() / ((double)RAND_MAX));

            // couleur bleuté
            glm::vec3 couleur = glm::vec3(0, 0.2, (glm::mix(0.15, 0.9, rand() / ((float)RAND_MAX))));
            if (i < 5) // les 5 premières ont une couleur spéciale pour bien les identifier
            {
                glm::vec3 couleurs[5] = { { 1 , 0 , 0.1 }, { 1 , 0.6 , 0.1 }, { 1 , 0 , 0.8 }, { 0.1, .8, 0.1 }, { 0.7 , 0.6 , 1 } }; // rouge, orangé , rose/mauve , vert , violet pale
                couleur = couleurs[i];
            }

            // créer une nouvelle planete
            Exoplanete* exoplanete = new Exoplanete(rayon, hauteur[i], angle, vit, taille, couleur);

            // ajouter cette planete dans la liste
            exoplanetes.push_back(exoplanete);
        }

        // créer quelques autres formes
        glUseProgram(progBase);
        if (cubeFil == NULL) cubeFil = new FormeCube(1.0, false);
        if (quad == NULL) quad = new FormeQuad(1.0, true);
    }
    ~TrouDeVer()
    {
        conclureGraphique();
        // vider la scene
        while (!exoplanetes.empty()) exoplanetes.pop_back();
    }
    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
        if (prog <= 0)
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ((locVertex = glGetAttribLocation(prog, "Vertex")) == -1) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ((locColor = glGetAttribLocation(prog, "Color")) == -1) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ((locillumination = glGetUniformLocation(prog, "illumination")) == -1) std::cerr << "!!! pas trouvé la \"Location\" de illumination" << std::endl;
        if ((locmonochromacite = glGetUniformLocation(prog, "monochromacite")) == -1) std::cerr << "!!! pas trouvé la \"Location\" de monochromacite" << std::endl;
    }
    void conclureGraphique()
    {
        delete cubeFil;
        delete quad;
    }

    void afficherParoiXpos()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            matrModel.Translate(0.5, 0.0, 0.0);
            matrModel.Rotate(-90.0, 0.0, 1.0, 0.0);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherParoiXneg()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            matrModel.Translate(-0.5, 0.0, 0.0);
            matrModel.Rotate(90.0, 0.0, 1.0, 0.0);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherParoiYpos()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            matrModel.Translate(0.0, 0.5, 0.0);
            matrModel.Rotate(90.0, 1.0, 0.0, 0.0);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherParoiYneg()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            matrModel.Translate(0.0, -0.5, 0.0);
            matrModel.Rotate(-90.0, 1.0, 0.0, 0.0);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherParoiZneg()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            matrModel.Translate(0.0, 0.0, -0.5);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherParoiZpos()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            matrModel.Translate(0.0, 0.0, 0.5);
            matrModel.Rotate(-180.0, 1.0, 0.0, 0.0);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherCoins()
    {
        matrModel.PushMatrix(); {
            matrModel.Scale(2 * Etat::whDim.x, 2 * Etat::whDim.y, 2 * Etat::whDim.z);
            glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
            cubeFil->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
    }
    void afficherParois()
    {
        // tracer les coins du trou de ver avec le programme de base
        glEnable(GL_DEPTH_TEST);
        glVertexAttrib3f(locColorBase, 1.0, 1.0, 1.0); // blanc
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        afficherCoins();

        // tracer les distorsions gravitationnelle du trou de ver
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE); glCullFace(GL_BACK); // ne pas afficher les faces arrière

        glVertexAttrib4f(locColorBase, 0.5, 0.2, 0.2, 0.15); // rougeâtre
        afficherParoiXpos(); // paroi en +X

        glVertexAttrib4f(locColorBase, 0.2, 0.5, 0.2, 0.15); // verdâtre
        afficherParoiZpos(); // paroi en +Z

        glVertexAttrib4f(locColorBase, 0.2, 0.2, 0.5, 0.15); // bleuté
        afficherParoiXneg(); // paroi en -X

        glVertexAttrib4f(locColorBase, 0.5, 0.5, 0.2, 0.15); // jaunâtre
        afficherParoiZneg(); // paroi en -Z

        glVertexAttrib4f(locColorBase, 0.7, 0.7, 0.7, 0.15); // gris pâle, plus opaque
        afficherParoiYpos(); // paroi en -Y

        glVertexAttrib4f(locColorBase, 0.4, 0.4, 0.4, 0.15); // gris foncé, plus opaque
        afficherParoiYneg(); // paroi en -Y

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

    void positionnerCamera(int exoplaneteChoisie)
    {
        // partie 2: modifs ici ...
        Exoplanete *exoplanete = exoplanetes[exoplaneteChoisie-1];
        MatricePipeline mtc = exoplanete->obtenirMatriceCourante();
        matrVisu.setMatr( glm::inverse( glm::mat4(mtc) ) );
    }

    void afficherToutesLesExoplanetes()
    {
        glVertexAttrib4f(locColor, 1.0, 1.0, 1.0, 1.0);

        for (std::vector<Exoplanete*>::iterator it = exoplanetes.begin(); it != exoplanetes.end(); it++)
        {
            (*it)->afficher();
        }
    }

    // afficher le contenu de la galaxie lointaine dans le trou de ver
    void afficherContenu(GLenum ordre = GL_CCW)
    {
        glUseProgram(prog);
        // TODO: ...

        // afficher des triangles en plein ou en fil de fer ?
        glPolygonMode(GL_FRONT_AND_BACK, Etat::modePolygone);
        // ne pas afficher les triangles dont on voit la face arrière ? (touche 'c')
        if (Etat::culling) glEnable(GL_CULL_FACE);
        if (Etat::culling) glCullFace(ordre == GL_CCW ? GL_BACK : GL_FRONT);

        // afficher les planetes
        afficherToutesLesExoplanetes();

        if (Etat::culling) glDisable(GL_CULL_FACE);
    }

    void afficher()
    {
        // tracer le trou de ver avec le programme de nuanceur de base
        glUseProgram(progBase);
        glUniformMatrix4fv(locmatrProjBase, 1, GL_FALSE, matrProj);
        glUniformMatrix4fv(locmatrVisuBase, 1, GL_FALSE, matrVisu);
        glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // 1) Remplir le stencil avec la "lentille" en affichant les parois de
        // façon similaire à ce qui est fait dans afficherParois().
        // On veut des 1,2,4,8 ou 16 pour tous les pixels des lentilles et
        // des 0 partout ailleurs, mais on ne veut rien tracer à l'écran
        // pour le moment: on s'organise pour que le test du stencil ne
        // passe jamais tout en remplissant le stencil de valeurs.

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NEVER, 1, 1);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

        glEnable(GL_CULL_FACE); 
        glCullFace(GL_BACK); // ne pas afficher les faces arrière
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        
        //partie 1: modifs ici ...
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE );
        glStencilFunc( GL_NEVER, 1, 1 );
        afficherParoiZpos(); // paroi en +Z

        glStencilFunc( GL_NEVER, 2, 2 );
        afficherParoiXpos(); // paroi en +X

        glStencilFunc( GL_NEVER, 4, 4 );
        afficherParoiZneg(); // paroi en -Z

        glStencilFunc( GL_NEVER, 8, 8 );
        afficherParoiXneg(); // paroi en -X

        glStencilFunc( GL_NEVER, 16, 16 );
        afficherParoiYneg(); // paroi en -Y

        glDisable(GL_CULL_FACE);


        // 2) Maintenant que le stencil est rempli de valeurs aux positions des lentilles,
        // on trace les planetes de la galaxie lointaine.
        // On fait en sorte que seulement la région du stencil avec des 1,2,4,8 soit tracée
        // (c'est-à-dire seulement la région de lentille)
        // tracer les planetes avec le programme de nuanceur de ce TP
        glUseProgram(prog);
        glUniformMatrix4fv(locmatrProj, 1, GL_FALSE, matrProj);
        glUniformMatrix4fv(locmatrVisu, 1, GL_FALSE, matrVisu);
        glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
        glUniform1i(locillumination, Etat::illumination);
        glUniform1i(locmonochromacite, Etat::monochromacite);

        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

        // partie 1: modifs ici ...
        // [ au besoin, utiliser : if ( Etat::debug ) glStencilFunc( GL_ALWAYS, 1, 1 ); // pour débogguer ]
        // on trace le contenu de chaque lentille 5 fois
        glStencilFunc( GL_EQUAL, 1, 1 );
        afficherContenu();
        glStencilFunc( GL_EQUAL, 2, 2 );
        afficherContenu();
        glStencilFunc( GL_EQUAL, 4, 4 );
        afficherContenu();
        glStencilFunc( GL_EQUAL, 8, 8 );
        afficherContenu();
        glUseProgram(progBase);
        
        // afficher les parois du trou de ver
        glDisable(GL_STENCIL_TEST);
        afficherParois();
        // lorsqu'on a passer dans le trou de ver, nous voyons l'ensemble des planètes
        if (Etat::exoplaneteChoisie) {
            afficherContenu();
        }

    }
    
    void afficherExosoleil() {
        // tracer le soleil avec le programme de nuanceur de base
        glUseProgram(progBase);
        glUniformMatrix4fv(locmatrProjBase, 1, GL_FALSE, matrProj);
        glUniformMatrix4fv(locmatrVisuBase, 1, GL_FALSE, matrVisu);
        glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);

        if (soleil == NULL) soleil = new FormeSphere(3.5, 25, 25, true);

        glVertexAttrib3f(locColor, (glm::mix(0.75, 0.85, rand() / ((float)RAND_MAX))), (glm::mix(0.25, 0.30, rand() / ((float)RAND_MAX))), 0.0);
        matrModel.PushMatrix();
            matrModel.Rotate(45*Etat::dt, 0.0, 1.0, 0.0);
            matrModel.PushMatrix();
                matrModel.Scale(1.5, 1.5, 0.1);
                glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
                soleil->afficher();
            matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
        
            matrModel.PushMatrix();
                matrModel.Scale(1.5, 0.1, 1.5);
                glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
                soleil->afficher();
            matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);

            matrModel.PushMatrix();
                glVertexAttrib3f(locColor, 0.0, 0.0, 0.0);
                matrModel.Scale(.5, 0.5, 0.5);
                glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
                soleil->afficher();
            matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);
        matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModelBase, 1, GL_FALSE, matrModel);

    }

    void calculerPhysique( )
    {
        if ( Etat::enmouvement )
        {
            if ( getenv("DEMO") != NULL )
            {
                // faire varier la taille du trou de ver automatiquement pour la démo
                static int sensX = 1;
                Etat::whDim.x += sensX * 0.1 * Etat::dt;
                if ( Etat::whDim.x < 8.0 ) sensX = +1;
                else if ( Etat::whDim.x > 12.0 ) sensX = -1;

                static int sensZ = -1;
                Etat::whDim.z += sensZ * 0.07 * Etat::dt;
                if ( Etat::whDim.z < 8.0 ) sensZ = +1;
                else if ( Etat::whDim.z > 12.0 ) sensZ = -1;
            }

            for ( std::vector<Exoplanete*>::iterator it = exoplanetes.begin() ; it != exoplanetes.end() ; it++ )
            {
                (*it)->avancerPhysique();
            }
        }
    }

    GLint locillumination;
    GLint locmonochromacite;
    // la liste des isocaedre
    std::vector<Exoplanete*> exoplanetes;
};

FormeCube* TrouDeVer::cubeFil = NULL;
FormeQuad* TrouDeVer::quad = NULL;
// Dans votre quête du tp2, nous vous donnons certains indice ...
FormeSphere* TrouDeVer::soleil = NULL; // Parie 1 2 et 3, À compléter 

#endif

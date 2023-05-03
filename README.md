# Notre décodeur JPEG à nous

Bienvenue sur la page d'accueil de _votre_ projet JPEG, un grand espace de liberté, sous le regard bienveillant de vos enseignants préférés.
Le sujet sera disponible dès le mardi 2 mai à l'adresse suivante : [https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/](https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/).

Comme indiqué lors de l'amphi de présentation, vous devrez organiser un point d'étape avec vos enseignants pour valider cette architecture logicielle.
Cette page d'accueil servira de base à cette discussion. En pratique, vous pouvez reprendre son contenu comme bon vous semble, mais elle devra au moins comporter les infos suivantes :

1. des informations sur le découpage des fonctionnalités du projet en modules, en spécifiant les données en entrée et sortie de chaque étape ;
2. (au moins) un dessin des structures de données de votre projet (format libre, ça peut être une photo d'un dessin manuscrit par exemple) ;
3. une répartition des tâches au sein de votre équipe de développement, comportant une estimation du temps consacré à chacune d'elle (là encore, format libre, du truc cracra fait à la main, au joli Gantt chart).

Rajouter **régulièrement** des informations sur l'avancement de votre projet est aussi **une très bonne idée** (prendre 10 min tous les trois chaque matin pour résumer ce qui a été fait la veille, établir un plan d'action pour la journée qui commence et reporter tout ça ici, par exemple).

# Liens utiles

- Bien former ses messages de commits : [https://www.conventionalcommits.org/en/v1.0.0/](https://www.conventionalcommits.org/en/v1.0.0/) ;
- Problème relationnel au sein du groupe ? Contactez [Pascal](https://fr.wikipedia.org/wiki/Pascal,_le_grand_fr%C3%A8re) !
- Besoin de prendre l'air ? Le [Mont Rachais](https://fr.wikipedia.org/wiki/Mont_Rachais) est accessible à pieds depuis la salle E301 !
- Un peu juste sur le projet à quelques heures de la deadline ? Le [Montrachet](https://www.vinatis.com/achat-vin-puligny-montrachet) peut faire passer l'envie à vos profs de vous mettre une tôle !

- **OUR PROJECT**
    # 1ST STEP OF THE PROJECT: MODULES AND SPECIFICATIONS
        -MODE BASELINE:
            * PROGRAM ENTRY: IMAGE FILE JPEG  -> OUT FILE: PPM IMAGE OR PGM IMAGE
                -executable has the name "jpeg2ppm"
            
            file structure
            
                jpeg_reader.c
                    * JPEG head extraction:
                    -image size extraction (SOF)  -> height x width
                    -sampling factor extraction (SOF)  -> in [1..4] 
                    -components number N (SOF) -> N between 1 and 3
                    -quantification table extraction (DQT) -> precision , i_Q , table values
                    -Huffman tables (DHT) -> symbols number by length ( 1 to 16 bits ) and symbols		
                decoded_image_creator.c
                    *create_image_ppm or pgm:
                        -specify image_name and type (.ppm or .pgm (color))
                treatment.c
                    submodul:
                        -Huffman.c
                        -MCU_decoder.c
                        -toRGB.c
                        -idct.c
                        -zigzag_quantification.c
                main.c
                    -read image ".jpeg" ou ".jpg"
                    -call jpeg_reader
                        ...treatment...
                    -close image
                    -call create_image_ppm
                    

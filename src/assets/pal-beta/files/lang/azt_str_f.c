#include <ultra64.h>

char *lang[] = {
	/*  0*/ "|Briefing - \n\nEn suivant la derni�re manoeuvre d'Elvis, les trois appareils impliqu�s se sont �cras�s ensemble en Alaska. La position du Pr�sident est incertaine et un �cran de brouillage CME emp�che toute action de sauvetage ou de localisation du site.\n\n|Carrington - \n\nPour l'instant nous ne pouvons que vous envoyer des messages, Joanna. Si vous parvenez � �liminer la source du brouillage, nous pourrons d�terminer votre position et envoyer de l'aide. Trouver et prot�ger le Pr�sident demeure en revanche votre priorit� absolue.\n\n|Objectif 1: - R�cup�rer scanner m�dical pr�sidentiel\n\nCet appareil actualise en permanence le journal m�dical du Pr�sident et lui assure un �tat de sant� stable. Trouvez- le pour garder le Pr�sident en bonne sant�.\n\n|Objectif 2: - Activer le signal de d�tresse\n\nCelui-ci ne sera efficace qu'une fois le brouillage �limin�mais s'il est activ� au point d'�vacuation, les �quipes de sauvetage arriveront plus vite.\n\n|Objectif 3: - D�sactiver appareil brouillage ennemi\n\nUn syst�me ennemi sophistiqu� inonde le secteur d'un brouillage CME imp�n�trable. Vous pourrez d�terminer sa position plus facilement au sol que nos appareils en altitude. Une fois trouv�, coupez le courant et nous vous fournirons de l'aide.\n\n|Objectif 4: - Eliminer le clone du Pr�sident\n\nLa s�curit� des Etats Unis serait menac�e si deux individus identiques ayant tout pouvoir affirmaient �tre le Pr�sident. Identifiez l'imposteur et �liminez la menace.\n\n|Objectif 5: - Trouver et sauver le Pr�sident\n\nCet objectif est votre priorit�. La s�curit� du Pr�sident est vitale. Il vous faut le prot�ger � tout prix avant l'arriv�e de renforts ou �carter tout danger.\n\nFIN\n",
	/*  1*/ "SITE du CRASH\n",
	/*  2*/ "|Briefing - \n\nEn suivant la derni�re manoeuvre d'Elvis, les trois appareils impliqu�s se sont �cras�s ensemble en Alaska. La position du Pr�sident est incertaine et un �cran de brouillage CME emp�che toute action de sauvetage ou de localisation du site.\n\n|Carrington - \n\nPour l'instant nous ne pouvons que vous envoyer des messages, Joanna. Si vous parvenez � �liminer la source du brouillage, nous pourrons d�terminer votre position et envoyer de l'aide. Trouver et prot�ger le Pr�sident demeure en revanche votre priorit� absolue.\n\n|Objectif 1: - R�cup�rer scanner m�dical pr�sidentiel\n\nCet appareil actualise en permanence le journal m�dical du Pr�sident et lui assure un �tat de sant� stable. Trouvez- le pour garder le Pr�sident en bonne sant�.\n\n|Objectif 2: - Activer le signal de d�tresse\n\nCelui-ci ne sera efficace qu'une fois le brouillage �limin�mais s'il est activ� au point d'�vacuation, les �quipes de sauvetage arriveront plus vite.\n\n|Objectif 3: - Eliminer le clone du Pr�sident\n\nLa s�curit� des Etats Unis serait menac�e si deux individus identiques ayant tout pouvoir affirmaient �tre le Pr�sident. Identifiez l'imposteur et �liminez la menace.\n\n|Objectif 4: - Trouver et sauver le Pr�sident\n\nCet objectif est votre priorit�. La s�curit� du Pr�sident est vitale. Il vous faut le prot�ger � tout prix avant l'arriv�e de renforts ou �carter tout danger.\n\nFIN\n",
	/*  3*/ "|Briefing - \n\nEn suivant la derni�re manoeuvre d'Elvis, les trois appareils impliqu�s se sont �cras�s ensemble en Alaska. La position du Pr�sident est incertaine et un �cran de brouillage CME emp�che toute action de sauvetage ou de localisation du site.\n\n|Carrington - \n\nPour l'instant nous ne pouvons que vous envoyer des messages, Joanna. Si vous parvenez � �liminer la source du brouillage, nous pourrons d�terminer votre position et envoyer de l'aide. Trouver et prot�ger le Pr�sident demeure en revanche votre priorit� absolue.\n\n|Objectif 1: - Activer le signal de d�tresse\n\nCelui-ci ne sera efficace qu'une fois le brouillage �limin�mais s'il est activ� au point d'�vacuation, les �quipes de sauvetage arriveront plus vite.\n\n|Objectif 2: - Eliminer le clone du Pr�sident\n\nLa s�curit� des Etats Unis serait menac�e si deux individus identiques ayant tout pouvoir affirmaient �tre le Pr�sident. Identifiez l'imposteur et �liminez la menace.\n\n|Objectif 3: - Trouver et sauver le Pr�sident\n\nCet objectif est votre priorit�. La s�curit� du Pr�sident est vitale. Il vous faut le prot�ger � tout prix avant l'arriv�e de renforts ou �carter tout danger.\n\nFIN\n",
	/*  4*/ "\n",
	/*  5*/ "R�cup�rer scanner m�dical pr�sidentiel\n",
	/*  6*/ "Activer le signal de d�tresse\n",
	/*  7*/ "D�sactiver appareil brouillage ennemi\n",
	/*  8*/ "Eliminer le clone du Pr�sident\n",
	/*  9*/ "Trouver et sauver le Pr�sident\n",
	/* 10*/ "Voil� le point d'�vacuation.\n",
	/* 11*/ "Le signal peut venir de l�.\n",
	/* 12*/ "Le brouillage... Il vient de ce vaisseau.\n",
	/* 13*/ "Elvis... Il pourra prot�ger le Pr�sident.\n",
	/* 14*/ "Signal de d�tresse activ�.\n",
	/* 15*/ "Objet crucial d�truit.\n",
	/* 16*/ "Obtenir le scanner pr�sidentiel.\n",
	/* 17*/ "\n",
	/* 18*/ "\n",
	/* 19*/ "Un Scanner pr�sidentiel\n",
	/* 20*/ "Scanner pr�sidentiel obtenu.\n",
	/* 21*/ "Appareil de brouillage d�sactiv�.\n",
	/* 22*/ "Le Pr�sident a �t� abattu.\n",
	/* 23*/ "Clone pr�sidentiel �limin�.\n",
	/* 24*/ "Vous ne pourrez pas le sauver!\n",
	/* 25*/ "Pr�sident sauv�.\n",
	/* 26*/ "Obtenir Vision nocturne.\n",
	/* 27*/ "Cassandra De Vries: \n",
	/* 28*/ "Vision nocturne\n",
	/* 29*/ "Vision nocturne\n",
	/* 30*/ "Vision nocturne obtenue.\n",
	/* 31*/ "Prenez ceci. Cela vous sera utile!\n",
	/* 32*/ "Ahhh... oohhhh...\n",
	/* 33*/ "Ahhh... ohh... ouuuuhhh...\n",
	/* 34*/ "Agent Dark! Quel est votre statut?\n",
	/* 35*/ "Perfect Dark, r�pondez!\n",
	/* 36*/ "A-agent Dark au rapport...\n",
	/* 37*/ "Agent Dark! Parlez bon sang!\n",
	/* 38*/ "Quelque chose brouille mes transmissions... Cela vient peut-�tre de l'autre appareil.\n",
	/* 39*/ "Je ferais mieux de trouver la source du brouillage et m'assurer qu'Elvis est en bonne sant�. Sans oublier le Pr�sident...\n",
	/* 40*/ "Comment vous sentez-vous, M. le Pr�sident?\n",
	/* 41*/ "Mieux. Merci mon petit. Je me souviendrai de cette journ�e. Comment Easton a-t-il pu faire une chose pareille? Je le savais ambitieux, mais tout ceci...\n",
	/* 42*/ "Une question, monsieur. Qu'est-ce que le Pelagic II, cette chose que Trent cherchait?\n",
	/* 43*/ "Un vaisseau de recherche sous-marin du gouvernement am�ricain. Un mod�le unique pouvant mener des exp�riences � de grandes profondeurs. Trent a formul� une demande de pr�t au nom de la dataDyne Corporation. J'ai refus�.\n",
	/* 44*/ "Trent devra r�pondre de ses actes. Encore faut-il le trouver.\n",
	/* 45*/ "Vous avez �chou�, Easton. Vous �tes un �l�ment corrompu. Nous nous passerons de vous.\n",
	/* 46*/ "C'est �a oui. P�tasse su�doise, va!\n",
	/* 47*/ "Noooooooon!!!\n",
};
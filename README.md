Nempont Jérôme M2 ILJ - Faculté Jean-Perrin

Projet qui consiste à convertir une image vectorielle en une image matricielle

step 1 : make
step 2 : export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:<path>/minwin/bin
step 3 : bin/test <file.obj>
step 4 (optional) : make clean

commande globale: 
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:<path>/minwin/bin && make clean && make && bin/test assets/teapot.obj 

TOUCHES:
	Z - Avancer
	S - Reculer
	Q - Gauche
	D - Droite

	A - Monter
	E - Descendre

	T - Axe X ACW
	G - Axe X CW
	F - Axe Y ACW
	H - Axe Y CW
	R - Axe Z ACW
	Y - Axe Z CW



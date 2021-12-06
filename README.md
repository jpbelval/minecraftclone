# ClusterBlock

> :warning: Projet compilé sur Linux. Output dans le dossier build

> ℹ️ Voir branche Windows pour un exécutable

## Fonctionnement
ClusterBlock ne fonctionne qu'en ligne. Une adresse IP est requise lors du démarrage du client. Une version build du serveur se situe dans server/build/ .

Pour build serveur, aller dans le dossier server

>g++ main.cpp server.cpp server.h ../playerdata.cpp ../playerdata.h -lenet -o build/server.out



## Dépendances
- SFML
- GLU
- GLEW
- DevIL
- ENet


---
# **MyDiscord (EN/FR):**

### EN:

# **A home-grown version of Discord**

#### a real-time messaging application with an interface

## App features:

- every time a user logs in they find the **message history,** and know **the author** and **exact time** of each message

- users can **create channels** as they wish

- users can **react** to each message with an emoji

## Assignment Brief:

Before entering the the main application for the first time, each
visitor gives their identity: **surname, first name, e-mail address**
and **password**.

The password must be chosen with care - **complex, unique** and hidden from view. And, once transmitted, it never remains unencrypted: **it is immediately encrypted**, then kept safe in the database, rendering any attempt at intrusion futile.

Once registered, the user lands in the main channel, ready to chat.
Users are able to navigate freely between channels, both **public** and **private**. And a simple **logout button** can take the user to the home page.

🎭 There is **a role management system - member, moderator, administrator -** with each type of user knowing their rights to **create a channel**, **delete one**, or **ban** an undesirable one.
ensuite
🧵 So that users can talk at the same time, the server relies on **threads.** Each connection should open a fluid, autonomous thread of execution.

🔌 The application uses **sockets;** tunnels **linking the server to the client**. The server, always listening on its port, waits for users to connect, and sends or receives messages.

🗃 The application must use a relational database called **myDiscord.sql** and use **PostgreSQL** to manage users, activity, authorizations, and carefully guard each message. The database must be more than a logbook: it needs to embody **memory**, **logic** and **order** in the chaos of discussions.

🔒 In private channels, certain messages must remain secret. A simple
but effective encryption system (such as an **XOR** or **symmetrical algorithm**) must ensure that only authorized eyes can read the message.

## Plan to add later: 

- [ ] File transfer - Enable users to send files (images, documents,
    etc.).

- [ ] User status - Online, away, busy, etc.

- [ ] User mentions - Implement a system for mentioning other users with @.

- [ ] Message encryption - Implement simple encryption for private
    messages

### FR :

# Une version maison de Discord

#### une application de messagerie en temps réel avec une interface

## Caractéristiques de l\'application :

-   chaque fois qu\'un utilisateur se connecte, il trouve **l\'historique des messages** et connaît **l\'auteur** et **l\'heure exacte** de chaque message

-   les utilisateurs peuvent **créer des canaux** comme ils le souhaitent

-   les utilisateurs peuvent **réagir** à chaque message avec un emoji

## Résumé de la mission :

Avant d\'entrer pour la première fois dans l\'application principale,
chaque visiteur donne son identité : **nom, prénom, adresse e-mail** et **mot de passe**.

Le mot de passe doit être choisi avec soin - **complexe, unique et caché.** Une fois transmis, il ne reste jamais en clair : **il est immédiatement chiffré,** puis conservé en toute sécurité dans la base de données, rendant toute tentative d'intrusion vaine.

Une fois enregistré, l\'utilisateur accède au canal principal, prêt à
discuter. Les utilisateurs peuvent naviguer librement entre les canaux,
qu\'ils soient **publics** ou **privés**. Et un simple **bouton de déconnexion** peut ramener l\'utilisateur à la page d\'accueil.

🎭 Il existe **un système de gestion des rôles - membre, modérateur, administrateur -** chaque type d\'utilisateur connaissant ses droits pour **créer un canal**, **en supprimer** un ou **bannir** un indésirable.

🧵 Pour que les utilisateurs puissent parler en même temps, le serveur s\'appuie sur les **threads**. Chaque connexion doit ouvrir un fil d\'exécution fluide et autonome.

🔌 L\'application utilise des **sockets** ; des **tunnels reliant le serveur au client**. Le serveur, toujours en écoute sur son port, attend que les utilisateurs se connectent, et envoie ou reçoit des messages.

🗃 L\'application doit utiliser une base de données relationnelle appelée **myDiscord.sql** et utiliser **PostgreSQL** pour gérer les utilisateurs, l\'activité, les autorisations et garder soigneusement chaque message. La base de données doit être plus qu\'un journal de bord : elle doit incarner la **mémoire**, la **logique** et **l\'ordre** dans le chaos des discussions.

🔒 Dans les canaux privés, certains messages doivent rester secrets. Un système de cryptage simple mais efficace (tel qu\'un **algorithme XOR ou symétrique**) doit garantir que seuls les yeux autorisés peuvent lire le message.

## A ajouter ultérieurement : 

- [ ] Transfert de fichiers - Permettre aux utilisateurs d\'envoyer des fichiers (images, documents, etc.).

- [ ] Statut de l\'utilisateur - En ligne, absent, occupé, etc.

- [ ] Mentions d\'utilisateurs - Mettre en place un système permettant de mentionner d\'autres utilisateurs avec @.

- [ ] Cryptage des messages - Mise en place d\'un cryptage simple pour les messages privés.
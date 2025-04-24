/* dictionaries.c */
#include "dictionaries.h"

/* English (default) translations */
Translations translations_en = {
.login_title = "Login to MyDiscord",
.register_title = "Sign-up to MyDiscord",
.login_subtitle = "Welcome back!",
.register_subtitle = "Enter your details to register.",
.name_placeholder = "Enter your first name",
.surname_placeholder = "Enter your surname",
.username_placeholder = "Enter a user name",
.email_placeholder = "Enter your email",
.password_placeholder = "Enter your password",
.login_button = "Login",
.logout_button = "Logout",
.invalid_message = "Invalid email or password.",
.create_account_msg = "Create new account",
.return_msg = "Return to login",
.register_button = "Register",
.return_button = "Back"
};
/* French translations */
Translations translations_fr = {
.login_title = "Connectez-vous à MyDiscord",
.register_title = "S'inscrire sur MyDiscord",
.login_subtitle = "Bon retour!",
.register_subtitle = "Entrez vos détails pour vous inscrire.",
.name_placeholder = "Entrer votre prénom",
.surname_placeholder = "Entrer votre nom de famille",
.username_placeholder = "Entrer un nom d'utilisateur",
.email_placeholder = "Entrez votre email",
.password_placeholder = "Entrez votre mot de passe",
.login_button = "Connexion",
.logout_button = "Déconnexion",
.invalid_message = "Email ou mot de passe invalide.",
.create_account_msg = "Créer un nouveau compte",
.return_msg = "Retour à la connexion",
.register_button = "S'enregistrer",
.return_button = "Retour"
};
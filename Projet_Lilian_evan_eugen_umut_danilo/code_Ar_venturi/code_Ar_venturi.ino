// ============================================================
//  COVAL LEMAX90X14S — Commande vide delta robot
//  Brochage :
//    PIN_CAPTEUR_FORCE  A0  → Capteur FSR / force (0–1023)
//    PIN_EV_VIDE         7  → Relais EV vide   (IN1 du module relais)
//    PIN_EV_SOUFFLAGE    8  → Relais EV soufflage (IN2)
//    LED_ETAT           13  → LED témoin état vide
// ============================================================

const int PIN_CAPTEUR_FORCE = A0;
const int PIN_EV_VIDE       = 7;
const int PIN_EV_SOUFFLAGE  = 8;
const int LED_ETAT          = 13;

// --- Paramètres réglables ---
const int   SEUIL_CONTACT      = 200;   // Valeur ADC déclenchant l'aspiration (0–1023)
const float DUREE_MAINTIEN_S   = 3.0;   // Secondes de maintien vide après dépôt
const int   DUREE_SOUFFLAGE_MS = 300;   // Durée impulsion soufflage (ms)
const int   DEBOUNCE_MS        = 50;    // Anti-rebond capteur (ms)

// --- États machine ---
enum Etat {
  ATTENTE_CONTACT,   // Robot descend, pas encore de pression
  ASPIRATION_ACTIVE, // Contact détecté → vide enclenché
  MAINTIEN_VIDE,     // Robot remonté → vide maintenu par ASC
  SOUFFLAGE,         // Relâchement de la pièce
  PAUSE_CYCLE        // Court délai avant prochain cycle
};

Etat etatCourant = ATTENTE_CONTACT;

unsigned long tDebut       = 0;
unsigned long tDebounce    = 0;
bool          contactActif = false;

// --- Pilotage relais (actif BAS sur la plupart des modules relais) ---
// Adapter RELAY_ON / RELAY_OFF si votre module est actif HAUT
const int RELAY_ON  = LOW;
const int RELAY_OFF = HIGH;

void evVideON()       { digitalWrite(PIN_EV_VIDE,      RELAY_ON);  }
void evVideOFF()      { digitalWrite(PIN_EV_VIDE,      RELAY_OFF); }
void evSoufflageON()  { digitalWrite(PIN_EV_SOUFFLAGE, RELAY_ON);  }
void evSoufflageOFF() { digitalWrite(PIN_EV_SOUFFLAGE, RELAY_OFF); }

// ============================================================
void setup() {
  pinMode(PIN_EV_VIDE,      OUTPUT);
  pinMode(PIN_EV_SOUFFLAGE, OUTPUT);
  pinMode(LED_ETAT,         OUTPUT);

  evVideOFF();
  evSoufflageOFF();
  digitalWrite(LED_ETAT, LOW);

  Serial.begin(9600);
  Serial.println("Systeme LEMAX pret.");
}

// ============================================================
void loop() {
  int valForce = analogRead(PIN_CAPTEUR_FORCE);
  unsigned long maintenant = millis();

  // ---- Lecture capteur avec anti-rebond ----
  if (valForce >= SEUIL_CONTACT) {
    if (!contactActif) {
      if (tDebounce == 0) tDebounce = maintenant;
      if (maintenant - tDebounce >= DEBOUNCE_MS) contactActif = true;
    }
  } else {
    contactActif = false;
    tDebounce    = 0;
  }

  // ---- Machine à états ----
  switch (etatCourant) {

    case ATTENTE_CONTACT:
      if (contactActif) {
        Serial.println("[1] Contact -> ASPIRATION");
        evVideON();
        digitalWrite(LED_ETAT, HIGH);
        tDebut      = maintenant;
        etatCourant = ASPIRATION_ACTIVE;
      }
      break;

    case ASPIRATION_ACTIVE:
      // On attend que le robot remonte (pression retombe)
      // OU que le vide soit établi (ici on laisse 500 ms puis on passe en maintien)
      if (!contactActif || (maintenant - tDebut >= 500)) {
        Serial.println("[2] Vide etabli -> MAINTIEN");
        // Le LEMAX ASC coupe l'air automatiquement — la sortie reste HIGH
        tDebut      = maintenant;
        etatCourant = MAINTIEN_VIDE;
      }
      break;

    case MAINTIEN_VIDE:
      // Vide maintenu par la soupape interne ASC du LEMAX
      if (maintenant - tDebut >= (unsigned long)(DUREE_MAINTIEN_S * 1000)) {
        Serial.println("[3] Fin maintien -> SOUFFLAGE");
        evVideOFF();
        evSoufflageON();
        tDebut      = maintenant;
        etatCourant = SOUFFLAGE;
      }
      break;

    case SOUFFLAGE:
      if (maintenant - tDebut >= DUREE_SOUFFLAGE_MS) {
        Serial.println("[4] Soufflage termine -> PAUSE");
        evSoufflageOFF();
        digitalWrite(LED_ETAT, LOW);
        tDebut      = maintenant;
        etatCourant = PAUSE_CYCLE;
      }
      break;

    case PAUSE_CYCLE:
      if (maintenant - tDebut >= 200) {
        Serial.println("[5] Pret pour prochain cycle");
        etatCourant = ATTENTE_CONTACT;
      }
      break;
  }

  // ---- Debug série ----
  static unsigned long tLog = 0;
  if (maintenant - tLog >= 200) {
    tLog = maintenant;
    Serial.print("Force="); Serial.print(valForce);
    Serial.print("  Etat="); Serial.println(etatCourant);
  }
}

#ifndef LANGUAGE_H
#define LANGUAGE_H

typedef enum
{
    LANGUAGE_ENGLISH,
    LANGUAGE_TURKISH,
    LANGUAGE_FRENCH,
    LANGUAGE_GERMAN,
    LANGUAGE_SPANISH,
    LANGUAGE_ITALIAN,
    LANGUAGE_PORTUGUESE,
    LANGUAGE_RUSSIAN,
    LANGUAGE_ARABIC,
} language_t;

//! PATHS
//? PAGE0
#define L_PAGE0_T0 "page0.l_device_version"
#define L_PAGE0_LANGUAGE_VAL "page0.lang"
#define L_PAGE0_SD_VAL "page0.sd"
#define L_PAGE0_TIME_VAL "page0.time"
#define L_PAGE0_WIFI_MODE_VAL "page0.wifi_mode"
#define L_PAGE0_B_MIN_VAL "page0.b_min"
#define L_PAGE0_B_PRESS_VAL "page0.b_press"
#define L_PAGE0_ON_VAL "page0.on"
#define L_PAGE0_OFF_VAL "page0.off"
#define L_PAGE0_L_DURATION "page0.l_duration"
#define L_PAGE0_L_PRESS "page0.l_press"
#define L_PAGE0_L_LEVEL "page0.l_level"
#define L_PAGE0_L_OKEY "page0.l_okey"

//? HOME
#define L_HOME_T1 "home.l_t1"
#define L_HOME_T2 "home.l_t2"
#define L_HOME_T3 "home.l_t3"

//?HOME_INFO
#define L_HOME_INFO_T1 "home_info.l_t1"
#define L_HOME_INFO_T2 "home_info.l_t2"

//?SETTINGS_1
#define L_SETTINGS_1_T1 "settings_1.l_t1"
#define L_SETTINGS_1_T2 "settings_1.l_t2"

//?SETTINGS_2
#define L_SETTINGS_2_T1 "settings_2.l_t1"
#define L_SETTINGS_2_T2 "settings_2.l_t2"

//?SETTINGS_3
#define L_SETTINGS_3_T1 "settings_3.l_t1"
#define L_SETTINGS_3_T2 "settings_3.l_t2"

//?SETTINGS_4
#define L_SETTINGS_4_T1 "settings_4.l_t1"
#define L_SETTINGS_4_T2 "settings_4.l_t2"

//?DEVICE_SET_1
#define L_DEVICE_SET_1_T1 "device_set_1.l_t1"
#define L_DEVICE_SET_1_T3 "device_set_1.l_t3"
#define L_DEVICE_SET_1_T4 "device_set_1.l_t4"

//?DEVICE_SET_2
#define L_DEVICE_SET_2_T1 "device_set_2.l_t1"
#define L_DEVICE_SET_2_T4 "device_set_2.l_t4"

//?DEVICE_SET_3

//?DATE
#define L_DATE_T0 "date.l_t0"
#define L_DATE_T1 "date.l_t1"
#define L_DATE_T3 "date.l_t3"

//?HOUR
#define L_HOUR_T3 "hour.l_t3"
#define L_HOUR_T4 "hour.l_t4"

//?APAP
#define L_APAP_T1 "apap.l_t1"
#define L_APAP_T8 "apap.l_t8"

//?START
#define L_START_T0 "start.l_t0"

//?INFO
#define L_INFO_T1 "info.l_t1"
#define L_INFO_T2 "info.l_t2"
#define L_INFO_T3 "info.l_t3"
#define L_INFO_T5 "info.l_t5"
#define L_INFO_T6 "info.l_t6"
#define L_INFO_T7 "info.l_t7"
#define L_INFO_T8 "info.l_t8"
#define L_INFO_T9 "info.l_t9"
#define L_INFO_T11 "info.l_t11"
#define L_INFO_T12 "info.l_t12"

//?RAPOR_1
#define L_RAPOR_1_B0 "rapor_1.l_b0"
#define L_RAPOR_1_B1 "rapor_1.l_b1"
#define L_RAPOR_1_B3 "rapor_1.l_b3"
#define L_RAPOR_1_B10 "rapor_1.l_b10"
#define L_RAPOR_1_B11 "rapor_1.l_b11"

//?COMFORT
#define L_COMFORT_T0 "comfort.l_t0"
#define L_COMFORT_T1 "comfort.l_t1"
#define L_COMFORT_T2 "comfort.l_t2"

//?MASK
#define L_MASK_T0 "mask.l_t0"

//?ALARM
#define L_ALARM_T0 "alarm.l_t0"

//?PASSWORD
#define L_PASSWORD_T0 "password.l_t0"
#define L_PASSWORD_T1 "password.l_t1"
//! PATHS

//*TÜRKÇE
#define L_TR_PAGE0_T0 "YEL Versiyon V.0.1 Elite"
#define L_TR_PAGE0_B_MIN_VAL "dk"
#define L_TR_PAGE0_B_PRESS_VAL "cmH2O"
#define L_TR_PAGE0_ON_VAL "Açık"
#define L_TR_PAGE0_OFF_VAL "Kapalı"
#define L_TR_PAGE0_L_DURATION "Süre"
#define L_TR_PAGE0_L_PRESS "Basınç"
#define L_TR_PAGE0_L_LEVEL "Seviye"
#define L_TR_PAGE0_L_OKEY "Tamam"

//? HOME
#define L_TR_HOME_T1 "Ana Sayfa"
#define L_TR_HOME_T2 "Ayarlar"
#define L_TR_HOME_T3 "Terapiyi Başlat"

//?HOME_INFO
#define L_TR_HOME_INFO_T1 "Alarm"
#define L_TR_HOME_INFO_T2 "Raporlar"

//?SETTINGS_1
#define L_TR_SETTINGS_1_T1 "Cihaz"
#define L_TR_SETTINGS_1_T2 "Klinik"

//?SETTINGS_2
#define L_TR_SETTINGS_2_T1 "Rampa"
#define L_TR_SETTINGS_2_T2 "Nemlendirici"

//?SETTINGS_3
#define L_TR_SETTINGS_3_T1 "Saat"
#define L_TR_SETTINGS_3_T2 "Tarih"

//?SETTINGS_4
#define L_TR_SETTINGS_4_T1 "Konfor"
#define L_TR_SETTINGS_4_T2 "Bilgi"

//?DEVICE_SET_1
#define L_TR_DEVICE_SET_1_T1 "Dil"
#define L_TR_DEVICE_SET_1_T3 "Ekran"
#define L_TR_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_TR_DEVICE_SET_2_T1 "Uyku Süresi"
#define L_TR_DEVICE_SET_2_T4 "Parlaklık"

//?DATE
#define L_TR_DATE_T0 "Gün"
#define L_TR_DATE_T1 "Ay"
#define L_TR_DATE_T3 "Yıl"

//?HOUR
#define L_TR_HOUR_T3 "Saat"
#define L_TR_HOUR_T4 "Dakika"

//?APAP
#define L_TR_APAP_T1 "Üst Basınç"
#define L_TR_APAP_T8 "Alt Basınç"

//?START
#define L_TR_START_T0 "Hedef Basınç:"

//?INFO
#define L_TR_INFO_T1 "Seri No:"
#define L_TR_INFO_T5 "Mac Adresi:"
#define L_TR_INFO_T2 "Yazılım Sürümü:"
#define L_TR_INFO_T3 "Üretim Tarihi:"
#define L_TR_INFO_T6 "Çalışma Süresi:"

//?RAPOR_1
#define L_TR_RAPOR_1_B0 "Son 10 Gün:"
#define L_TR_RAPOR_1_B1 "Son 10 Terapi"
#define L_TR_RAPOR_1_B3 "Terapiyi Bul"
#define L_TR_RAPOR_1_B10 "Son 1 Yıl"
#define L_TR_RAPOR_1_B11 "Son 10 Ay"

//?COMFORT
#define L_TR_COMFORT_T0 "   Otomatik \nRampa Kapatma"
#define L_TR_COMFORT_T1 "Rahat Nefes"
#define L_TR_COMFORT_T2 "Oto. Başlatma"

//?MASK
#define L_TR_MASK_T0 "Maske Çıktı"

//?ALARM
#define L_TR_ALARM_T0 "Maske Çıkarıldı!"

//?PASSWORD
#define L_TR_PASSWORD_T0 "Lütfen Şifre Girin!"
#define L_TR_PASSWORD_T1 "Şifre Hatalı!"

//! ////////////////////////////////////////////////////////////////////////////////////////////////////////

//*ENGLISH

#define L_EN_PAGE0_T0 "YEL Version V.0.1 Elite"
#define L_EN_PAGE0_B_MIN_VAL "min"
#define L_EN_PAGE0_B_PRESS_VAL "cmH2O"
#define L_EN_PAGE0_ON_VAL "On"
#define L_EN_PAGE0_OFF_VAL "Off"
#define L_EN_PAGE0_L_DURATION "Duration"
#define L_EN_PAGE0_L_PRESS "Pressure"
#define L_EN_PAGE0_L_LEVEL "Level"
#define L_EN_PAGE0_L_OKEY "OK"

//? HOME
#define L_EN_HOME_T1 "Home"
#define L_EN_HOME_T2 "Settings"
#define L_EN_HOME_T3 "Start Therapy"

//?HOME_INFO
#define L_EN_HOME_INFO_T1 "Alarm"
#define L_EN_HOME_INFO_T2 "Reports"

//?SETTINGS_1
#define L_EN_SETTINGS_1_T1 "Device"
#define L_EN_SETTINGS_1_T2 "Clinical"

//?SETTINGS_2
#define L_EN_SETTINGS_2_T1 "Ramp"
#define L_EN_SETTINGS_2_T2 "Humidifier"

//?SETTINGS_3
#define L_EN_SETTINGS_3_T1 "Time"
#define L_EN_SETTINGS_3_T2 "Date"

//?SETTINGS_4
#define L_EN_SETTINGS_4_T1 "Comfort"
#define L_EN_SETTINGS_4_T2 "Info"

//?DEVICE_SET_1
#define L_EN_DEVICE_SET_1_T1 "Language"
#define L_EN_DEVICE_SET_1_T3 "Display"
#define L_EN_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_EN_DEVICE_SET_2_T1 "Sleep Time"
#define L_EN_DEVICE_SET_2_T4 "Brightness"

//?DATE
#define L_EN_DATE_T0 "Day"
#define L_EN_DATE_T1 "Month"
#define L_EN_DATE_T3 "Year"

//?HOUR
#define L_EN_HOUR_T3 "Hour"
#define L_EN_HOUR_T4 "Minute"

//?APAP
#define L_EN_APAP_T1 "Max Pressure"
#define L_EN_APAP_T8 "Min Pressure"

//?START
#define L_EN_START_T0 "Target Pressure:"

//?INFO
#define L_EN_INFO_T1 "Serial No:"
#define L_EN_INFO_T5 "MAC Address:"
#define L_EN_INFO_T2 "Software Version:"
#define L_EN_INFO_T3 "Manufacture Date:"
#define L_EN_INFO_T6 "Working Time:"

//?RAPOR_1
#define L_EN_RAPOR_1_B0 "Last 10 Days:"
#define L_EN_RAPOR_1_B1 "Last 10 Therapies"
#define L_EN_RAPOR_1_B3 "Find Therapy"
#define L_EN_RAPOR_1_B10 "Last 1 Year"
#define L_EN_RAPOR_1_B11 "Last 10 Months"

//?COMFORT
#define L_EN_COMFORT_T0 "   Auto \nRamp Off"
#define L_EN_COMFORT_T1 "Easy Breathing"
#define L_EN_COMFORT_T2 "Auto Start"

//?MASK
#define L_EN_MASK_T0 "Mask Removed"

//?ALARM
#define L_EN_ALARM_T0 "Mask Taken Off!"

//?PASSWORD
#define L_EN_PASSWORD_T0 "Please Enter Password!"
#define L_EN_PASSWORD_T1 "Incorrect Password!"

//*GERMAN

#define L_DE_PAGE0_T0 "YEL Version V.0.1 Elite"
#define L_DE_PAGE0_B_MIN_VAL "Min"
#define L_DE_PAGE0_B_PRESS_VAL "cmH2O"
#define L_DE_PAGE0_ON_VAL "Ein"
#define L_DE_PAGE0_OFF_VAL "Aus"
#define L_DE_PAGE0_L_DURATION "Dauer"
#define L_DE_PAGE0_L_PRESS "Druck"
#define L_DE_PAGE0_L_LEVEL "Stufe"
#define L_DE_PAGE0_L_OKEY "OK"

//? HOME
#define L_DE_HOME_T1 "Startseite"
#define L_DE_HOME_T2 "Einstellungen"
#define L_DE_HOME_T3 "Therapie Starten"

//?HOME_INFO
#define L_DE_HOME_INFO_T1 "Alarm"
#define L_DE_HOME_INFO_T2 "Berichte"

//?SETTINGS_1
#define L_DE_SETTINGS_1_T1 "Gerät"
#define L_DE_SETTINGS_1_T2 "Klinisch"

//?SETTINGS_2
#define L_DE_SETTINGS_2_T1 "Rampe"
#define L_DE_SETTINGS_2_T2 "Befeuchter"

//?SETTINGS_3
#define L_DE_SETTINGS_3_T1 "Uhrzeit"
#define L_DE_SETTINGS_3_T2 "Datum"

//?SETTINGS_4
#define L_DE_SETTINGS_4_T1 "Komfort"
#define L_DE_SETTINGS_4_T2 "Information"

//?DEVICE_SET_1
#define L_DE_DEVICE_SET_1_T1 "Sprache"
#define L_DE_DEVICE_SET_1_T3 "Bildschirm"
#define L_DE_DEVICE_SET_1_T4 "WLAN"

//?DEVICE_SET_2
#define L_DE_DEVICE_SET_2_T1 "Schlafzeit"
#define L_DE_DEVICE_SET_2_T4 "Helligkeit"

//?DATE
#define L_DE_DATE_T0 "Tag"
#define L_DE_DATE_T1 "Monat"
#define L_DE_DATE_T3 "Jahr"

//?HOUR
#define L_DE_HOUR_T3 "Stunde"
#define L_DE_HOUR_T4 "Minute"

//?APAP
#define L_DE_APAP_T1 "Max. Druck"
#define L_DE_APAP_T8 "Min. Druck"

//?START
#define L_DE_START_T0 "Zieldruck:"

//?INFO
#define L_DE_INFO_T1 "Serien-Nr:"
#define L_DE_INFO_T5 "MAC-Adresse:"
#define L_DE_INFO_T2 "Softwareversion:"
#define L_DE_INFO_T3 "Herstellungsdatum:"
#define L_DE_INFO_T6 "Betriebszeit:"

//?RAPOR_1
#define L_DE_RAPOR_1_B0 "Letzte 10 Tage:"
#define L_DE_RAPOR_1_B1 "Letzte 10 Therapien"
#define L_DE_RAPOR_1_B3 "Therapie Finden"
#define L_DE_RAPOR_1_B10 "Letztes Jahr"
#define L_DE_RAPOR_1_B11 "Letzte 10 Monate"

//?COMFORT
#define L_DE_COMFORT_T0 "   Autom. \nRampe Aus"
#define L_DE_COMFORT_T1 "Angenehmes Atmen"
#define L_DE_COMFORT_T2 "Auto-Start"

//?MASK
#define L_DE_MASK_T0 "Maske Entfernt"

//?ALARM
#define L_DE_ALARM_T0 "Maske Abgenommen!"

//?PASSWORD
#define L_DE_PASSWORD_T0 "Bitte Passwort Eingeben!"
#define L_DE_PASSWORD_T1 "Falsches Passwort!"

//*FRENCH

#define L_FR_PAGE0_T0 "YEL Version V.0.1 Elite"
#define L_FR_PAGE0_B_MIN_VAL "min"
#define L_FR_PAGE0_B_PRESS_VAL "cmH2O"
#define L_FR_PAGE0_ON_VAL "Activé"
#define L_FR_PAGE0_OFF_VAL "Désactivé"
#define L_FR_PAGE0_L_DURATION "Durée"
#define L_FR_PAGE0_L_PRESS "Pression"
#define L_FR_PAGE0_L_LEVEL "Niveau"
#define L_FR_PAGE0_L_OKEY "OK"

//? HOME
#define L_FR_HOME_T1 "Accueil"
#define L_FR_HOME_T2 "Paramètres"
#define L_FR_HOME_T3 "Démarrer Thérapie"

//?HOME_INFO
#define L_FR_HOME_INFO_T1 "Alarme"
#define L_FR_HOME_INFO_T2 "Rapports"

//?SETTINGS_1
#define L_FR_SETTINGS_1_T1 "Appareil"
#define L_FR_SETTINGS_1_T2 "Clinique"

//?SETTINGS_2
#define L_FR_SETTINGS_2_T1 "Rampe"
#define L_FR_SETTINGS_2_T2 "Humidificateur"

//?SETTINGS_3
#define L_FR_SETTINGS_3_T1 "Heure"
#define L_FR_SETTINGS_3_T2 "Date"

//?SETTINGS_4
#define L_FR_SETTINGS_4_T1 "Confort"
#define L_FR_SETTINGS_4_T2 "Info"

//?DEVICE_SET_1
#define L_FR_DEVICE_SET_1_T1 "Langue"
#define L_FR_DEVICE_SET_1_T3 "Écran"
#define L_FR_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_FR_DEVICE_SET_2_T1 "Temps de Sommeil"
#define L_FR_DEVICE_SET_2_T4 "Luminosité"

//?DATE
#define L_FR_DATE_T0 "Jour"
#define L_FR_DATE_T1 "Mois"
#define L_FR_DATE_T3 "Année"

//?HOUR
#define L_FR_HOUR_T3 "Heure"
#define L_FR_HOUR_T4 "Minute"

//?APAP
#define L_FR_APAP_T1 "Pression Max"
#define L_FR_APAP_T8 "Pression Min"

//?START
#define L_FR_START_T0 "Pression Cible :"

//?INFO
#define L_FR_INFO_T1 "N° de Série :"
#define L_FR_INFO_T5 "Adresse MAC :"
#define L_FR_INFO_T2 "Version Logicielle :"
#define L_FR_INFO_T3 "Date de Fabrication :"
#define L_FR_INFO_T6 "Temps de Fonctionnement :"

//?RAPOR_1
#define L_FR_RAPOR_1_B0 "10 Derniers Jours :"
#define L_FR_RAPOR_1_B1 "10 Dernières Thérapies"
#define L_FR_RAPOR_1_B3 "Trouver Thérapie"
#define L_FR_RAPOR_1_B10 "Dernière Année"
#define L_FR_RAPOR_1_B11 "10 Derniers Mois"

//?COMFORT
#define L_FR_COMFORT_T0 "   Arrêt Auto \nRampe"
#define L_FR_COMFORT_T1 "Respiration Confortable"
#define L_FR_COMFORT_T2 "Démarrage Auto"

//?MASK
#define L_FR_MASK_T0 "Masque Retiré"

//?ALARM
#define L_FR_ALARM_T0 "Masque Enlevé !"

//?PASSWORD
#define L_FR_PASSWORD_T0 "Veuillez Entrer le Mot de Passe !"
#define L_FR_PASSWORD_T1 "Mot de Passe Incorrect !"

//*ITALIAN

#define L_IT_PAGE0_T0 "YEL Versione V.0.1 Elite"
#define L_IT_PAGE0_B_MIN_VAL "min"
#define L_IT_PAGE0_B_PRESS_VAL "cmH2O"
#define L_IT_PAGE0_ON_VAL "Attivo"
#define L_IT_PAGE0_OFF_VAL "Spento"
#define L_IT_PAGE0_L_DURATION "Durata"
#define L_IT_PAGE0_L_PRESS "Pressione"
#define L_IT_PAGE0_L_LEVEL "Livello"
#define L_IT_PAGE0_L_OKEY "OK"

//? HOME
#define L_IT_HOME_T1 "Home"
#define L_IT_HOME_T2 "Impostazioni"
#define L_IT_HOME_T3 "Avvia Terapia"

//?HOME_INFO
#define L_IT_HOME_INFO_T1 "Allarme"
#define L_IT_HOME_INFO_T2 "Report"

//?SETTINGS_1
#define L_IT_SETTINGS_1_T1 "Dispositivo"
#define L_IT_SETTINGS_1_T2 "Clinico"

//?SETTINGS_2
#define L_IT_SETTINGS_2_T1 "Rampa"
#define L_IT_SETTINGS_2_T2 "Umidificatore"

//?SETTINGS_3
#define L_IT_SETTINGS_3_T1 "Ora"
#define L_IT_SETTINGS_3_T2 "Data"

//?SETTINGS_4
#define L_IT_SETTINGS_4_T1 "Comfort"
#define L_IT_SETTINGS_4_T2 "Informazioni"

//?DEVICE_SET_1
#define L_IT_DEVICE_SET_1_T1 "Lingua"
#define L_IT_DEVICE_SET_1_T3 "Schermo"
#define L_IT_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_IT_DEVICE_SET_2_T1 "Tempo di Sospensione"
#define L_IT_DEVICE_SET_2_T4 "Luminosità"

//?DATE
#define L_IT_DATE_T0 "Giorno"
#define L_IT_DATE_T1 "Mese"
#define L_IT_DATE_T3 "Anno"

//?HOUR
#define L_IT_HOUR_T3 "Ora"
#define L_IT_HOUR_T4 "Minuto"

//?APAP
#define L_IT_APAP_T1 "Pressione Massima"
#define L_IT_APAP_T8 "Pressione Minima"

//?START
#define L_IT_START_T0 "Pressione Obiettivo:"

//?INFO
#define L_IT_INFO_T1 "Numero di Serie:"
#define L_IT_INFO_T5 "Indirizzo MAC:"
#define L_IT_INFO_T2 "Versione Software:"
#define L_IT_INFO_T3 "Data di Produzione:"
#define L_IT_INFO_T6 "Tempo di Funzionamento:"

//?RAPOR_1
#define L_IT_RAPOR_1_B0 "Ultimi 10 Giorni:"
#define L_IT_RAPOR_1_B1 "Ultime 10 Terapie"
#define L_IT_RAPOR_1_B3 "Trova Terapia"
#define L_IT_RAPOR_1_B10 "Ultimo Anno"
#define L_IT_RAPOR_1_B11 "Ultimi 10 Mesi"

//?COMFORT
#define L_IT_COMFORT_T0 "   Spegnimento \nAutomatico Rampa"
#define L_IT_COMFORT_T1 "Respiro Confortevole"
#define L_IT_COMFORT_T2 "Avvio Automatico"

//?MASK
#define L_IT_MASK_T0 "Maschera Rimosso"

//?ALARM
#define L_IT_ALARM_T0 "Maschera Rimosso!"

//?PASSWORD
#define L_IT_PASSWORD_T0 "Inserire la Password!"
#define L_IT_PASSWORD_T1 "Password Errata!"

//*PORTUGUESE

#define L_PT_PAGE0_T0 "YEL Versão V.0.1 Elite"
#define L_PT_PAGE0_B_MIN_VAL "min"
#define L_PT_PAGE0_B_PRESS_VAL "cmH2O"
#define L_PT_PAGE0_ON_VAL "Ligado"
#define L_PT_PAGE0_OFF_VAL "Desligado"
#define L_PT_PAGE0_L_DURATION "Duração"
#define L_PT_PAGE0_L_PRESS "Pressão"
#define L_PT_PAGE0_L_LEVEL "Nível"
#define L_PT_PAGE0_L_OKEY "OK"

//? HOME
#define L_PT_HOME_T1 "Página Inicial"
#define L_PT_HOME_T2 "Configurações"
#define L_PT_HOME_T3 "Iniciar Terapia"

//?HOME_INFO
#define L_PT_HOME_INFO_T1 "Alarme"
#define L_PT_HOME_INFO_T2 "Relatórios"

//?SETTINGS_1
#define L_PT_SETTINGS_1_T1 "Dispositivo"
#define L_PT_SETTINGS_1_T2 "Clínico"

//?SETTINGS_2
#define L_PT_SETTINGS_2_T1 "Rampa"
#define L_PT_SETTINGS_2_T2 "Umidificador"

//?SETTINGS_3
#define L_PT_SETTINGS_3_T1 "Hora"
#define L_PT_SETTINGS_3_T2 "Data"

//?SETTINGS_4
#define L_PT_SETTINGS_4_T1 "Conforto"
#define L_PT_SETTINGS_4_T2 "Informações"

//?DEVICE_SET_1
#define L_PT_DEVICE_SET_1_T1 "Idioma"
#define L_PT_DEVICE_SET_1_T3 "Tela"
#define L_PT_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_PT_DEVICE_SET_2_T1 "Tempo de Suspensão"
#define L_PT_DEVICE_SET_2_T4 "Brilho"

//?DATE
#define L_PT_DATE_T0 "Dia"
#define L_PT_DATE_T1 "Mês"
#define L_PT_DATE_T3 "Ano"

//?HOUR
#define L_PT_HOUR_T3 "Hora"
#define L_PT_HOUR_T4 "Minuto"

//?APAP
#define L_PT_APAP_T1 "Pressão Máxima"
#define L_PT_APAP_T8 "Pressão Mínima"

//?START
#define L_PT_START_T0 "Pressão Alvo:"

//?INFO
#define L_PT_INFO_T1 "Número de Série:"
#define L_PT_INFO_T5 "Endereço MAC:"
#define L_PT_INFO_T2 "Versão do Software:"
#define L_PT_INFO_T3 "Data de Fabricação:"
#define L_PT_INFO_T6 "Tempo de Funcionamento:"

//?RAPOR_1
#define L_PT_RAPOR_1_B0 "Últimos 10 Dias:"
#define L_PT_RAPOR_1_B1 "Últimas 10 Terapias"
#define L_PT_RAPOR_1_B3 "Buscar Terapia"
#define L_PT_RAPOR_1_B10 "Último Ano"
#define L_PT_RAPOR_1_B11 "Últimos 10 Meses"

//?COMFORT
#define L_PT_COMFORT_T0 "   Desligar Rampa \nAutomaticamente"
#define L_PT_COMFORT_T1 "Respiração Confortável"
#define L_PT_COMFORT_T2 "Início Automático"

//?MASK
#define L_PT_MASK_T0 "Máscara Removida"

//?ALARM
#define L_PT_ALARM_T0 "Máscara Removida!"

//?PASSWORD
#define L_PT_PASSWORD_T0 "Por favor, insira a senha!"
#define L_PT_PASSWORD_T1 "Senha Incorreta!"

//*RUSSIAN

#define L_RU_PAGE0_T0 "YEL Версия V.0.1 Elite"
#define L_RU_PAGE0_B_MIN_VAL "мин"
#define L_RU_PAGE0_B_PRESS_VAL "смH2O"
#define L_RU_PAGE0_ON_VAL "Вкл"
#define L_RU_PAGE0_OFF_VAL "Выкл"
#define L_RU_PAGE0_L_DURATION "Длительность"
#define L_RU_PAGE0_L_PRESS "Давление"
#define L_RU_PAGE0_L_LEVEL "Уровень"
#define L_RU_PAGE0_L_OKEY "ОК"

//? HOME
#define L_RU_HOME_T1 "Главная"
#define L_RU_HOME_T2 "Настройки"
#define L_RU_HOME_T3 "Начать терапию"

//?HOME_INFO
#define L_RU_HOME_INFO_T1 "Тревога"
#define L_RU_HOME_INFO_T2 "Отчёты"

//?SETTINGS_1
#define L_RU_SETTINGS_1_T1 "Устройство"
#define L_RU_SETTINGS_1_T2 "Клиника"

//?SETTINGS_2
#define L_RU_SETTINGS_2_T1 "Рампа"
#define L_RU_SETTINGS_2_T2 "Увлажнитель"

//?SETTINGS_3
#define L_RU_SETTINGS_3_T1 "Время"
#define L_RU_SETTINGS_3_T2 "Дата"

//?SETTINGS_4
#define L_RU_SETTINGS_4_T1 "Комфорт"
#define L_RU_SETTINGS_4_T2 "Информация"

//?DEVICE_SET_1
#define L_RU_DEVICE_SET_1_T1 "Язык"
#define L_RU_DEVICE_SET_1_T3 "Экран"
#define L_RU_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_RU_DEVICE_SET_2_T1 "Время ожидания"
#define L_RU_DEVICE_SET_2_T4 "Яркость"

//?DATE
#define L_RU_DATE_T0 "День"
#define L_RU_DATE_T1 "Месяц"
#define L_RU_DATE_T3 "Год"

//?HOUR
#define L_RU_HOUR_T3 "Час"
#define L_RU_HOUR_T4 "Минута"

//?APAP
#define L_RU_APAP_T1 "Макс. давление"
#define L_RU_APAP_T8 "Мин. давление"

//?START
#define L_RU_START_T0 "Целевое давление:"

//?INFO
#define L_RU_INFO_T1 "Серийный номер:"
#define L_RU_INFO_T5 "MAC-адрес:"
#define L_RU_INFO_T2 "Версия ПО:"
#define L_RU_INFO_T3 "Дата производства:"
#define L_RU_INFO_T6 "Время работы:"

//?RAPOR_1
#define L_RU_RAPOR_1_B0 "Последние 10 дней:"
#define L_RU_RAPOR_1_B1 "Последние 10 терапий"
#define L_RU_RAPOR_1_B3 "Найти терапию"
#define L_RU_RAPOR_1_B10 "Последний год"
#define L_RU_RAPOR_1_B11 "Последние 10 месяцев"

//?COMFORT
#define L_RU_COMFORT_T0 "   Авто. \nотключение рампы"
#define L_RU_COMFORT_T1 "Комфортное дыхание"
#define L_RU_COMFORT_T2 "Автостарт"

//?MASK
#define L_RU_MASK_T0 "Маска снята"

//?ALARM
#define L_RU_ALARM_T0 "Маска снята!"

//?PASSWORD
#define L_RU_PASSWORD_T0 "Пожалуйста, введите пароль!"
#define L_RU_PASSWORD_T1 "Неверный пароль!"

//*SPANISH

#define L_ES_PAGE0_T0 "YEL Versión V.0.1 Elite"
#define L_ES_PAGE0_B_MIN_VAL "min"
#define L_ES_PAGE0_B_PRESS_VAL "cmH2O"
#define L_ES_PAGE0_ON_VAL "Encendido"
#define L_ES_PAGE0_OFF_VAL "Apagado"
#define L_ES_PAGE0_L_DURATION "Duración"
#define L_ES_PAGE0_L_PRESS "Presión"
#define L_ES_PAGE0_L_LEVEL "Nivel"
#define L_ES_PAGE0_L_OKEY "Aceptar"

//? HOME
#define L_ES_HOME_T1 "Inicio"
#define L_ES_HOME_T2 "Configuración"
#define L_ES_HOME_T3 "Iniciar Terapia"

//?HOME_INFO
#define L_ES_HOME_INFO_T1 "Alarma"
#define L_ES_HOME_INFO_T2 "Informes"

//?SETTINGS_1
#define L_ES_SETTINGS_1_T1 "Dispositivo"
#define L_ES_SETTINGS_1_T2 "Clínico"

//?SETTINGS_2
#define L_ES_SETTINGS_2_T1 "Rampa"
#define L_ES_SETTINGS_2_T2 "Humidificador"

//?SETTINGS_3
#define L_ES_SETTINGS_3_T1 "Hora"
#define L_ES_SETTINGS_3_T2 "Fecha"

//?SETTINGS_4
#define L_ES_SETTINGS_4_T1 "Comodidad"
#define L_ES_SETTINGS_4_T2 "Información"

//?DEVICE_SET_1
#define L_ES_DEVICE_SET_1_T1 "Idioma"
#define L_ES_DEVICE_SET_1_T3 "Pantalla"
#define L_ES_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_ES_DEVICE_SET_2_T1 "Tiempo de espera"
#define L_ES_DEVICE_SET_2_T4 "Brillo"

//?DATE
#define L_ES_DATE_T0 "Día"
#define L_ES_DATE_T1 "Mes"
#define L_ES_DATE_T3 "Año"

//?HOUR
#define L_ES_HOUR_T3 "Hora"
#define L_ES_HOUR_T4 "Minuto"

//?APAP
#define L_ES_APAP_T1 "Presión máxima"
#define L_ES_APAP_T8 "Presión mínima"

//?START
#define L_ES_START_T0 "Presión objetivo:"

//?INFO
#define L_ES_INFO_T1 "N.º de serie:"
#define L_ES_INFO_T5 "Dirección MAC:"
#define L_ES_INFO_T2 "Versión del software:"
#define L_ES_INFO_T3 "Fecha de fabricación:"
#define L_ES_INFO_T6 "Tiempo de funcionamiento:"

//?RAPOR_1
#define L_ES_RAPOR_1_B0 "Últimos 10 días:"
#define L_ES_RAPOR_1_B1 "Últimas 10 terapias"
#define L_ES_RAPOR_1_B3 "Buscar terapia"
#define L_ES_RAPOR_1_B10 "Último año"
#define L_ES_RAPOR_1_B11 "Últimos 10 meses"

//?COMFORT
#define L_ES_COMFORT_T0 "   Apagado\n automático de rampa"
#define L_ES_COMFORT_T1 "Respiración cómoda"
#define L_ES_COMFORT_T2 "Inicio automático"

//?MASK
#define L_ES_MASK_T0 "Máscara retirada"

//?ALARM
#define L_ES_ALARM_T0 "¡Máscara retirada!"

//?PASSWORD
#define L_ES_PASSWORD_T0 "¡Por favor, introduzca la contraseña!"
#define L_ES_PASSWORD_T1 "¡Contraseña incorrecta!"

//*ARABIC

#define L_AR_PAGE0_T0 "إصدار YEL V.0.1 Elite"
#define L_AR_PAGE0_B_MIN_VAL "د"
#define L_AR_PAGE0_B_PRESS_VAL "cmH2O"
#define L_AR_PAGE0_ON_VAL "تشغيل"
#define L_AR_PAGE0_OFF_VAL "إيقاف"
#define L_AR_PAGE0_L_DURATION "المدة"
#define L_AR_PAGE0_L_PRESS "الضغط"
#define L_AR_PAGE0_L_LEVEL "المستوى"
#define L_AR_PAGE0_L_OKEY "موافق"

//? HOME
#define L_AR_HOME_T1 "الصفحة الرئيسية"
#define L_AR_HOME_T2 "الإعدادات"
#define L_AR_HOME_T3 "بدء العلاج"

//?HOME_INFO
#define L_AR_HOME_INFO_T1 "الإنذار"
#define L_AR_HOME_INFO_T2 "التقارير"

//?SETTINGS_1
#define L_AR_SETTINGS_1_T1 "الجهاز"
#define L_AR_SETTINGS_1_T2 "سريري"

//?SETTINGS_2
#define L_AR_SETTINGS_2_T1 "التمهيد"
#define L_AR_SETTINGS_2_T2 "المرطب"

//?SETTINGS_3
#define L_AR_SETTINGS_3_T1 "الساعة"
#define L_AR_SETTINGS_3_T2 "التاريخ"

//?SETTINGS_4
#define L_AR_SETTINGS_4_T1 "الراحة"
#define L_AR_SETTINGS_4_T2 "معلومات"

//?DEVICE_SET_1
#define L_AR_DEVICE_SET_1_T1 "اللغة"
#define L_AR_DEVICE_SET_1_T3 "الشاشة"
#define L_AR_DEVICE_SET_1_T4 "Wi-Fi"

//?DEVICE_SET_2
#define L_AR_DEVICE_SET_2_T1 "وقت الانتظار"
#define L_AR_DEVICE_SET_2_T4 "السطوع"

//?DATE
#define L_AR_DATE_T0 "اليوم"
#define L_AR_DATE_T1 "الشهر"
#define L_AR_DATE_T3 "السنة"

//?HOUR
#define L_AR_HOUR_T3 "الساعة"
#define L_AR_HOUR_T4 "الدقيقة"

//?APAP
#define L_AR_APAP_T1 "الضغط الأعلى"
#define L_AR_APAP_T8 "الضغط الأدنى"

//?START
#define L_AR_START_T0 "الضغط المستهدف:"

//?INFO
#define L_AR_INFO_T1 "الرقم التسلسلي:"
#define L_AR_INFO_T5 "عنوان MAC:"
#define L_AR_INFO_T2 "إصدار البرنامج:"
#define L_AR_INFO_T3 "تاريخ الإنتاج:"
#define L_AR_INFO_T6 "مدة التشغيل:"

//?RAPOR_1
#define L_AR_RAPOR_1_B0 "آخر 10 أيام:"
#define L_AR_RAPOR_1_B1 "آخر 10 علاجات"
#define L_AR_RAPOR_1_B3 "البحث عن علاج"
#define L_AR_RAPOR_1_B10 "آخر سنة"
#define L_AR_RAPOR_1_B11 "آخر 10 أشهر"

//?COMFORT
#define L_AR_COMFORT_T0 "   إيقاف تمهيد تلقائي"
#define L_AR_COMFORT_T1 "تنفس مريح"
#define L_AR_COMFORT_T2 "تشغيل تلقائي"

//?MASK
#define L_AR_MASK_T0 "تمت إزالة القناع"

//?ALARM
#define L_AR_ALARM_T0 "تمت إزالة القناع!"

//?PASSWORD
#define L_AR_PASSWORD_T0 "يرجى إدخال كلمة المرور!"
#define L_AR_PASSWORD_T1 "كلمة المرور غير صحيحة!"

void set_language(language_t language);

#endif
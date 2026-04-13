# Aperture Smart Relay (SDMUP)

## O projektu
Tento projekt představuje inteligentní řídicí modul pro automatizaci domácnosti, stylizovaný do prostředí laboratoří **Aperture Science**. Umožňuje bezdrátové ovládání tří nezávislých relé kanálů prostřednictvím responzivního webového rozhraní.

## Technologie
- **Hardware:** ESP8266 (NodeMCU V3), 3-kanálový relé modul.
- **Backend:** C++ (Arduino framework), ESP8266WebServer.
- **Frontend:** HTML5, CSS3 (Grid & Flexbox), JavaScript (AJAX/Fetch API).
- **Protokol:** JSON pro real-time synchronizaci stavu.

## Jak to funguje
1. Připojte modul k napájení.
2. Připojte se ke stejné Wi-Fi síti.
3. Zadejte IP adresu zařízení do prohlížeče.
4. Ovládejte spotřebiče v reálném čase bez obnovování stránky.

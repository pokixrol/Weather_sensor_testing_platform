# Weather sensor testing platform

>Karolína Říčná, VŠB-TUO 2025/26

Tanslation (EN)
>"There is no Internet of Things without a weather station, no guide is complete without a weather station, and if you haven't built a weather station, you haven't built anything. Everyone has built a weather station, and those who say they haven't are still building them today."

Original (CZ)
>"Bez meteostanice není internetu věcí, bez meteostanice není žádný návod kompletní a pokud jste si nepostavili meteostanici, nepostavili jste nic. Každý si postavil meteostanici, a kdo říká, že ne, staví ji dodnes."
>
>  ESP32 prakticky: Od základních obvodů k pokročilým aplikacím, Malý Martin

## Description
Original (CZ)
Obsahem této práce je návrh a realizace platformy pro testování meteorologických čidel založené na mikrokontroleru ESP32 a jednodeskovém počítači Raspberry Pi 5. Hardwarová stanice využívá digitální senzory k měření požadovaných veličin komunikující prostřednictvím sběrnice I2C. Komunikace mezi hardwarovou platformou a serverovou částí probíhá pomocí MQTT protokolu. Serverová část transformuje, ukládá a umožňuje zobrazení naměřených dat.

Tanslation (EN)
The content of this thesis is design and implementation of a platform for thesting weather sensors based on the ESP32 microcontroller and Raspberry Pi 5 single-board computer. he hardware station uses digital sensors to measure the required quantities, communicating via the I2C bus. Communication between the hardware platform and the server-side application is carried out using the MQTT protocol. The server component processes, stores, and enables visualization of the measured data.

## Hardware
Senosrs:
  - BME688 - Temperature, humidity, air preassure
  - BMP280 - Temperature, air pressure
  - SHT45 - Temperature, humidity
  - SHT40 - Temperature, humidity
  - BH1750 - Light intensity
  - TSL2561 - Light intensity
  - LTR390 - Ultra Violet light intenstiy
  - AS3935 - Lightning detector

Other:
  - ESP32-LPKit - Development board
  - CH9102 - Programmer

## Sources
  - ESP32 prakticky: Od základních obvodů k pokročilým aplikacím, Malý Martin











